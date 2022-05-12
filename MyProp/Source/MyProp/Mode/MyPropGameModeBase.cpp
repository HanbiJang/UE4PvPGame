// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/GameInstance/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/Controller/MyPlayerController.h>

AMyPropGameModeBase::AMyPropGameModeBase():
	GameLeftTimeSec(60.f * 7),
	maxPlayerNum( 3/*5*/ ),
	IsGameStartEnable(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AMyPropGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateTimerUI(DeltaTime);
}

void AMyPropGameModeBase::UpdateTimerUI_Implementation(float DeltaTime) {
	//게임 시작
	if (GI->iPlayerCnt == maxPlayerNum && !IsGameStartEnable) {
		IsGameStartEnable = true;
		UE_LOG(LogTemp, Log, TEXT("Start Game!!!!"));
	}

	//게임 타이머 ==============================================================
	//타이머 UI 업데이트 ========================================================
	if (IsGameStartEnable) {

		//시간 업데이트
		if (GameLeftTimeSec > 0) {
			GameLeftTimeSec = FMath::Clamp(GameLeftTimeSec - DeltaTime, 0.f, 60 * 99.f);
			if (GameLeftTimeSec <= 0) {
				GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Game Over"));
			}
		}
		//시간 분,초로 계산
		FString timeStr;
		int min = GameLeftTimeSec / 60.f;
		int sec = GameLeftTimeSec - min * 60;
		//2자리수로 시간 표시하기
		if (min <= 9) {
			if (sec <= 9) timeStr = FString::Printf(TEXT("0%i:0%i"), min, sec);
			else timeStr = FString::Printf(TEXT("0%i:%i"), min, sec);
		}
		else {
			if (sec <= 9) timeStr = FString::Printf(TEXT("%i:0%i"), min, sec);
			else timeStr = FString::Printf(TEXT("%i:%i"), min, sec);
		}

		//서버장 타이머 UI 업데이트
		//UI가 안그려졌을 때 대비...
		if(PCKiller->GetKillerMainHUD() && PCKiller->GetKillerMainHUD()->GetTimerHUD())
			PCKiller->GetKillerMainHUD()->GetTimerHUD()->SetTimeText(timeStr);
		//모든 클라 플레이어 UI 업데이트
		for (int i = 0; i < PCSurvivorArr.Num(); i++) {
			//UI가 클라 전용이라 UMG를 그냥 가져와서 쓸수가 없음.클라 전용에서만 실행되는 함수면 가능
			PCSurvivorArr[i]->UpdateTimerUI_Client(timeStr);
		}
	}
}

void AMyPropGameModeBase::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	
	if(GI == nullptr){
		GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
	if (GI) {
		//플레이어 인원 수 증가
		GI->iPlayerCnt++;
		UE_LOG(LogTemp, Log, TEXT("player Num : %i"), GI->iPlayerCnt);

		//캐릭터 스폰 인포
		FActorSpawnParameters spawnInfo;
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//플레이어가 다 모이면, 서버는 살인마, 클라는 생존자로 설정
		//스폰한 캐릭터에 설정 (빙의)
		if (GI->iPlayerCnt == 1) {
			//캐릭터 스폰 
			AKiller* pkiller = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(),
				vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);

			PCKiller = Cast<AMyPlayerController>(NewPlayer);
			if (PCKiller && pkiller) {
				PCKiller->Possess(pkiller); //캐릭터 possess
				PCKiller->DrawHUD_Server(); //HUD 설정 
			}

		}

		//클라이언트 - 생존자 캐릭터 설정
		else if (GI->iPlayerCnt >= 2) {
			//캐릭터 스폰
			//캐릭터 스폰 장소
			FVector fvspawnlo = FVector(
				vSurvivor1SpawnLocation.X,
				vSurvivor1SpawnLocation.Y + (GI->iPlayerCnt - 2) * 100,
				vSurvivor1SpawnLocation.Z
			);
			ASurvivor* psur = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(),
				fvspawnlo, FRotator(0, 0, 0), spawnInfo);

			AMyPlayerController* pcsur = Cast<AMyPlayerController>(NewPlayer);
			if (pcsur && psur) {
				PCSurvivorArr.Add(pcsur);
				pcsur->Possess(psur); //캐릭터 possess
				pcsur->DrawHUD_Client(); //HUD 설정 
			}
		}
	}
	else {
		//타이틀로 돌아가기
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartMap"));
	}

}