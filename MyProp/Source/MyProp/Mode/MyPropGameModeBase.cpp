// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>
#include <MyProp/MyPlayerController.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/MyPlayerController.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	DefaultPawnClass = nullptr;

	//UI
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();
		m_KillerMainHUDClass = GI->GetKillerWidgetClass();
	}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();
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
		if (GI->iPlayerCnt == 1) { //서버 - 살인마 캐릭터 설정
			pKillerPlayer = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(), vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);
			GetWorld()->GetFirstPlayerController()->Possess(pKillerPlayer); //서버 캐릭터 설정

			//UI 설정
			m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
			if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();

			// InputMode 설정
			APlayerController* Controller = GetWorld()->GetFirstPlayerController();
			FInputModeGameAndUI mode;
			Controller->SetInputMode(mode);
			Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.
		}
		else if (GI->iPlayerCnt == 2) { //클라이언트 - 생존자 캐릭터 설정
			pSurvivorPlayer1 = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(), vSurvivor1SpawnLocation, FRotator(0, 0, 0), spawnInfo);
			NewPlayer->Possess(pSurvivorPlayer1);
			
			//HUD 설정 
			AMyPlayerController* newPlayer_ = Cast<AMyPlayerController>(NewPlayer);
			if (newPlayer_ != nullptr)
				newPlayer_->DrawHUD_Client();

			// InputMode 설정
			APlayerController* Controller = NewPlayer;
			FInputModeGameAndUI mode;
			Controller->SetInputMode(mode); //변경필요
			Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.
		}
		
	}
}