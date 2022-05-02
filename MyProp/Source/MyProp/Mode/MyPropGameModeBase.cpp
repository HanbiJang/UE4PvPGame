// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/MyPlayerController.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	DefaultPawnClass = nullptr;

	////UI
	//UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI != nullptr) {
	//	m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();
	//	m_KillerMainHUDClass = GI->GetKillerWidgetClass();
	//}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();
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
			FString str = FString::Printf(TEXT("You Server"));

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, str); // 화면출력
			pKillerPlayer = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(), vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);
			GetWorld()->GetFirstPlayerController()->Possess(pKillerPlayer); //서버 캐릭터 설정

			//HUD 설정 
			Killer1Controller = Cast<AMyPlayerController>(NewPlayer);
			if (Killer1Controller != nullptr)
				Killer1Controller->DrawHUD_Server();
		}
		else if (GI->iPlayerCnt == 2) { //클라이언트 - 생존자 캐릭터 설정
			pSurvivorPlayer1 = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(), vSurvivor1SpawnLocation, FRotator(0, 0, 0), spawnInfo);
			NewPlayer->Possess(pSurvivorPlayer1);
			
			//HUD 설정 
			Survivor1Controller = Cast<AMyPlayerController>(NewPlayer);
			if (Survivor1Controller != nullptr)
				Survivor1Controller->DrawHUD_Client();
		}
	}
}