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
class UMyMatchingHUD;
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

	//플레이어 이름 && 스타트 화면에서 설정됨
	FString MyPlayerName = "DefaultName";

	FString GetMyPlayerName() { return MyPlayerName; }
	void SetMyPlayerName(FString str) { MyPlayerName = str; }

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

public:
	UFUNCTION(Reliable, Client)
		void UpdateTimerUI_Client(const FString& timestr, int DoneMachineNum);
	UFUNCTION(Reliable, Server)
		void UpdateTimerUI_Server(const FString& timestr, int DoneMachineNum);


	//매칭 ------
	//HUD
	TSubclassOf<UUserWidget> m_MatchingMainHUDClass;
	UMyMatchingHUD* m_MatchingMainHUD;
	UMyMatchingHUD* GetMatchingMainHUD() { return m_MatchingMainHUD; };

	void PosessToPawn(); //같은 카메라 pawn 에게 할당되도록 하기

	//플레이어 수
	int iPlayerCnt;
	void SetPlayerCnt(int n) { iPlayerCnt = n; }
	
	UFUNCTION(Reliable, Client)
		void SetPlayerCnt_Client(int n);

	int GetPlayerCnt() { return iPlayerCnt;  }

	UFUNCTION(Reliable, NetMulticast)
		void UpdatePlayerNum(int num); //UI 상에서 플레이어 수를 반영하기

	int ID;
	bool IsMatchingHUDSet;

public:
	void SetID(int i) { ID = i; }
	int GetID() { return ID; }

	UFUNCTION(Reliable, Client)
		void SetID_Client(int n);

};
