// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {

	//생존자 메인UI 가져오기
	ConstructorHelpers::FClassFinder<UUserWidget> SurvivorMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'")); //_C 포함해주기!
	//살인마 메인UI
	ConstructorHelpers::FClassFinder<UUserWidget> KillerMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/Killer/BP_KillerMainHUD.BP_KillerMainHUD_C'")); //_C 포함해주기!
	
	DefaultPawnClass = nullptr;

	//UI
	m_SurvivorMainHUDClass = SurvivorMainHUD.Class;
	m_KillerMainHUDClass = KillerMainHUD.Class;

	////선택 상태 가져오기
	//EPLAYER_TYPE m_SelectType;
	//UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI != nullptr) {
	//	m_SelectType = GI->GetSelectType();
	//	//[1] 생존자 코드
	//	switch (m_SelectType)
	//	{
	//	case EPLAYER_TYPE::KILLER:
	//		if (HasAuthority()) {
	//			DefaultPawnClass = GI->GetKiller();
	//			if (KillerMainHUD.Succeeded()) m_KillerMainHUDClass = KillerMainHUD.Class; //UI
	//		}
	//		break;
	//	case EPLAYER_TYPE::SURVIVOR:
	//		if (!HasAuthority()) {
	//			DefaultPawnClass = GI->GetSurvivor();
	//			if (SurvivorMainHUD.Succeeded()) m_SurvivorMainHUDClass = SurvivorMainHUD.Class; //UI
	//		}
	//		break;
	//	}
	//}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//메인 UI 설정하기
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {

		//if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
		//	m_MainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
		//	if (nullptr != m_MainHUD) m_MainHUD->AddToViewport();
		//}
		//else {
		//	m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
		//	if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
		//}	
	}

	// InputMode 설정
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI mode;

	Controller->SetInputMode(mode);
	Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.
}

void AMyPropGameModeBase::UpdatePlayHUD(float _CurHPRatio, float _CurSPRatio) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			//체력 UI
			UMyHPBarWidget* pHPHUD = m_SurvivorMainHUD->GetHPHUD();

			//스테미나 UI
			UMySPWidget* pSPHUD = m_SurvivorMainHUD->GetSPHUD();

			if (pHPHUD) {
				pHPHUD->SetHP(_CurHPRatio);
				pHPHUD->SetText(TEXT("Survivor 1"));
			}
			if (pSPHUD) {
				pSPHUD->SetSP(_CurSPRatio);
			}
		}
		else {

		}
	}
}

void AMyPropGameModeBase::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	//플레이어 인원 수 세기
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->iPlayerCnt++;
		FString str = FString::Printf(TEXT("Plyer Count : %d"), GI->iPlayerCnt);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, str); // 화면출력
	}

	//캐릭터
	FActorSpawnParameters spawnInfo; //스폰 인포
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	if (GI->iPlayerCnt >= 1) {

		//플레이어가 다 모이면, 서버는 살인마, 클라는 생존자로 설정

		//스폰한 캐릭터에 설정 (빙의)
		if (GI->iPlayerCnt == 1) {
			pKillerPlayer = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(), vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);
			GetWorld()->GetFirstPlayerController()->Possess(pKillerPlayer); //서버 캐릭터 설정
		}
		else if (GI->iPlayerCnt >= 2) { //클라이언트 캐릭터 설정
			pSurvivorPlayer1 = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(), vSurvivor1SpawnLocation, FRotator(0, 0, 0), spawnInfo);
			NewPlayer->Possess(pSurvivorPlayer1);
		}
		if (HasAuthority()) {
			m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
			if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
		}
		else {
			m_SurvivorMainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_SurvivorMainHUDClass));
			if (nullptr != m_SurvivorMainHUD) m_SurvivorMainHUD->AddToViewport();
		}
	}


}