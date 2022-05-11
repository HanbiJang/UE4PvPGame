// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/UI/MyMainHUD.h>
#include <MyProp/Player/Survivor/Survivor.h>

#include <MyProp/UI/Killer/MyKillerMainHUD.h>
#include <MyProp/Player/Killer/Killer.h>

#include <MyProp/UI/MyMachineWidget.h>
#include <MyProp/Machine/MyMachine.h>

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AMyPropGameModeBase;
/**
 * 
 */
UCLASS()
class MYPROP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
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
	virtual void PlayerTick(float DeltaTime) override;

public:
	//살인마 UI 업데이트
	UFUNCTION(Reliable, Server) //구현해야함
		void UpdatePlayHUD_Killer(float _CurQTimeRatio, float _CurETimeRatio, float _CurRCTimeRatio
			, float _CurQTime, float _CurETime, float _CurRCTime);

	UFUNCTION(Reliable, Client)
		void UpdatePlayHUD_Survivor(float _CurHPRatio, float _CurSPRatio, float _CurHP, float _MaxHP);

	UFUNCTION(Reliable, Client)
		void UpdateMachineHUD_Survivor(float _CurMachineRatio);

};
