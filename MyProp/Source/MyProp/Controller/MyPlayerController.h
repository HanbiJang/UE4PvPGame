// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/UI/MyMainHUD.h>
#include <MyProp/Player/Survivor/Survivor.h>

#include <MyProp/UI/Killer/MyKillerMainHUD.h>
#include <MyProp/Player/Killer/Killer.h>

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AMyMachine;
class UMyTimerWidget;
class AMyPropGameModeBase;
/**
 * 
 */
UCLASS()
class MYPROP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

		AMyPlayerController(); //생성자
public:
	UFUNCTION(Client, reliable)
		void DrawHUD_Client();

	UFUNCTION(Server, reliable)
		void DrawHUD_Server();

	//HUD
	TSubclassOf<UUserWidget> m_SurvivorMainHUDClass;
	UMyMainHUD* m_SurvivorMainHUD;
	UMyMainHUD* GetMainHUD(){ return m_SurvivorMainHUD; };

	//Killer (Server) HUD
	TSubclassOf<UUserWidget> m_KillerMainHUDClass;
	UMyKillerMainHUD* m_KillerMainHUD;
	UMyKillerMainHUD* GetKillerMainHUD() { return m_KillerMainHUD; };

public:
	/*virtual*/ void PlayerTick(float DeltaTime) override;
	void BeginPlay() override;

public:
	//살인마 UI 업데이트
	UFUNCTION(Reliable, Server) //구현해야함
		void UpdatePlayHUD_Killer(float _CurQTimeRatio, float _CurETimeRatio, float _CurRCTimeRatio
			, float _CurQTime, float _CurETime, float _CurRCTime);

	UFUNCTION(Reliable, Client)
		void UpdatePlayHUD_Survivor(float _CurHPRatio, float _CurSPRatio, float _CurHP, float _MaxHP);

	UFUNCTION(Reliable, Client)
		void UpdateMachineHUD_Survivor(float _CurMachineRatio);

private:
	//발전기 갯수 컨트롤 & UI
	int DoneMachineNum;
	bool IsAllMachineRepaired;

public:
	int GetDoneMachineNum() { return DoneMachineNum; }
	bool GetIsAllMachineRepaired() { return IsAllMachineRepaired; }
	void SetMachineDone(bool b) {
		DoneMachineNum++;
	}

	//게임 타이머======================================================
	
	//FTimerHandle FGameTimer; //게임 타이머
//private:
	//float GameLeftTimeSec; //게임 남은 시간을 초로 환산한 시간 : 7분
	//void CheckTime(); //게임 타이머 시간 체크
public:
	//float GetGameLeftTimeSec() { return GameLeftTimeSec; }
	//void SetGameLeftTimeSec(float value) { GameLeftTimeSec = value; }
	
	UFUNCTION(Reliable, Client)
		void UpdateTimerUI_Client(const FString& timestr);

};
