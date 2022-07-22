// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include <MyProp/UI/MyStartGameWidget.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/Controller/MyPlayerController.h>
#include <MyProp/Mode/MyStartModeBase.h>
#include <MyProp/Controller/MyMatchingController.h>


void UMyStartGameWidget::RandomBtnPressed() {
	//색 변화
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	//GetRandomBtn()->BackgroundColor = FLinearColor(1.0f,0.1f,0.1f); //red 안됨


	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->SetSelectType(EPLAYER_TYPE::RANDOM);
	}
}
void UMyStartGameWidget::KillerBtnPressed() {
	//색 변화
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->SetSelectType(EPLAYER_TYPE::KILLER);
	}
}
void UMyStartGameWidget::SurvivorBtnPressed() {
	//색 변화
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->SetSelectType(EPLAYER_TYPE::SURVIVOR);
	}
}
void UMyStartGameWidget::StartGameBtnPressed() {

	//색변화
	GetStartGameBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));

	//셀렉트 정보 가져오기
	EPLAYER_TYPE m_SelectType;
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SelectType = GI->GetSelectType();

		if (m_SelectType == EPLAYER_TYPE::RANDOM) {
			int irandom = rand() % 2;
			if (irandom == 0) {
				//살인마로 설정
				GI->SetSelectType(EPLAYER_TYPE::KILLER);
			}
			else {
				//생존자로 선택 설정
				GI->SetSelectType(EPLAYER_TYPE::SURVIVOR);
			}
			m_SelectType = GI->GetSelectType();
		}	
		//로딩 화면 보여주기
		m_LoadingHUD = (CreateWidget(GetWorld(), GI->m_LoadingWidget));
		if (nullptr != m_LoadingHUD) {
			m_LoadingHUD->AddToViewport();
		}

	}


	//선택한 캐릭터 타입에 따라 서버를 생성하거나 서버에 참여하기(찾기)
	if (m_SelectType == EPLAYER_TYPE::KILLER) {
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGameMap"), true, TEXT("Listen")); //서버
		
		CreateServer();
	}
		

	else if (m_SelectType == EPLAYER_TYPE::SURVIVOR) {
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1")); //로컬 클라이언트
		//로딩중 UI

		FindServer();
	}
}
//서버 만들기 (킬러)
void UMyStartGameWidget::CreateServer() {

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if(GI) SessionSettings.NumPublicConnections = GI->maxPlayer;
	else SessionSettings.NumPublicConnections = 5;

	//비동기 로딩 세션 생성 전에 미리 해야함
	//게임맵
	//LoadPackageAsync(TEXT("/Game/Levels/InGameMap"),
	//	FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type result)
	//		{
	//			if (result == EAsyncLoadingResult::Succeeded) {
	//				OnLoadingComplete_Server(SessionSettings);
	//			}
	//		}
	//	),
	//	0,
	//			PKG_ContainsMap);

	//매칭맵
	LoadPackageAsync(TEXT("/Game/Levels/Prototype/MatchingMap"),
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type result)
			{
				if (result == EAsyncLoadingResult::Succeeded) {
					OnLoadingComplete_Server(SessionSettings);
				}
			}
		),
		0,
				PKG_ContainsMap);

	//SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}
//서버 찾기 (생존자) 
void UMyStartGameWidget::FindServer() {

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true; //Is Lan
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Find Session Start"));
}

void UMyStartGameWidget::Init() {
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			//바인딩 - Server - 게임 세션 Create가 완료되었을 때
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnCreateSessionComplete);
			//바인딩  - Client - 게임 세션 Find가 완료되었을 때
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnFindSessionComplete);
			//바인딩 - Client - 게임에 Join이 완료되었을 때 
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnJoinSessionComplete);
		}
	}
}

void UMyStartGameWidget::OnCreateSessionComplete(FName ServerName, bool Succeded) {
	
	if (Succeded) {


		//UE_LOG(LogTemp, Log, TEXT("Create %s Session Success: %d"), *ServerName.ToString(), Succeded);
		//"World'/Game/Levels/InGameMap.InGameMap'" (원래 주소) listen =서버로 동작
		//GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
		
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Blue, TEXT("Create Session Success")); // Server 생성 성공
		GetWorld()->ServerTravel("/Game/Levels/Prototype/MatchingMap?listen"); //매칭 화면으로 건너가기
	}
}

#pragma region asynchronous-loading

void UMyStartGameWidget::OnLoadingComplete_Server(FOnlineSessionSettings SessionSettings) {
	//(레벨) 로딩 완료되었을 때의 함수
	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
	
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGameMap"),true,(FString)(L"listen"));

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("OnLoadingComplete_Server"));
	//GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");

}

//void UMyStartGameWidget::OnLoadingComplete_Client(APlayerController* pc, FString joinAddress, ETravelType type) {
//	
//	//로딩 완료되었을 때의 함수
//	//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Game Start!"));
//	Cast<AMyPlayerController>(pc)->ClientTravel(joinAddress, type);
//
//}

//void UMyStartGameWidget::OnLoadingComplete(const FName&, UPackage*, EAsyncLoadingResult::Type result) {
//	//로딩 완료되었을 때의 함수
//	if (result == EAsyncLoadingResult::Succeeded) {
//		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("succee"));
//		GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
//		//GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandler, this, &UMyStartGameWidget::DelayServerTravel, 3.0f, false);
//	}
//	else {
//		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Fail"));
//	}
//	
//}


//void UMyStartGameWidget::DelayServerTravel() {
//
//	//로딩 완료되었을 때의 함수
//	GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
//
//}

//void UMyStartGameWidget::DelayClientTravel() {
//	
//	//내용 없음
//}

#pragma endregion

void UMyStartGameWidget::OnFindSessionComplete(bool Succeded) {
	
	if (Succeded) {

		TArray<FOnlineSessionSearchResult> SearchResults;

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Find Session Success"));
		FString str = FString::Printf(TEXT("Serch result : server count %d"), SearchResults.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, str);

		SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num() >= 1)  //Find 결과가 있다면
		{ 	
			SessionInterface->JoinSession(0, "My Session", SearchResults[0]); //문제 : 여러 서버가 있다면 멀티플레이를 서버밖에 못함
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("JoinSession() Start"));
		}
		else { //Find 결과 없다면 다시 Find 하기
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No Server to Join"));
			FindServer();
			//SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		}

;
	}	

}

void UMyStartGameWidget::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Join Session Complete"));

	//StartLevel 월드 셋팅에서 AMyPlayerController로 컨트롤러 변경
	APlayerController* playerCont = UGameplayStatics::GetPlayerController(GetWorld(), 0); //플레이어 컨트롤러 가져오기 => StartMap의 컨트롤러는 AMyController 타입	

	if (playerCont) {
		
		AMyPlayerController* m_PC = Cast<AMyPlayerController>(playerCont);

		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName , JoinAddress);

		if (JoinAddress != "") { //Join할 서버의 주소를 찾았을 경우

			if (m_PC) {

				////플레이어 이름 설정
				//UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
				//if (GI) {
				//	Cast<AMyPlayerController>(PC)->SetMyPlayerName(GI->MyPlayerName);
				//}
				
				////비동기 로딩
				//LoadPackageAsync(TEXT("/Game/Levels/InGameMap"),
				//	FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type result)
				//		{
				//			if (result == EAsyncLoadingResult::Succeeded) {
				//				Cast<AMyPlayerController>(PC)->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
				//				//OnLoadingComplete_Client(PC, JoinAddress, ETravelType::TRAVEL_Absolute);
				//			}
				//			else {
				//				GEngine->AddOnScreenDebugMessage(0, 5, FColor::Blue, TEXT("Join Failed"));

				//				//다시 찾기
				//				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
				//			}
				//		}
				//	),
				//	0,
				//	PKG_ContainsMap);

				
				m_PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Join Complete - Client Travel Start"));

			}
			else 
			{ 
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("playerCont Can't Cast to AMyPlayerController")); 
			}			
			
		}
		else //Join Address를 얻어내지 못했음
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("JoinAddress null"));
		}
			
	}

}