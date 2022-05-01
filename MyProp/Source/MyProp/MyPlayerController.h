// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/UI/MyMainHUD.h>
//#include <MyProp/UI/MyHPBarWidget.h>
//#include <MyProp/UI/MySPWidget.h>
#include <MyProp/Player/Survivor/Survivor.h>

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

	//HUD
	TSubclassOf<UUserWidget> m_SurvivorMainHUDClass;
	UMyMainHUD* m_SurvivorMainHUD;

	UMyMainHUD* GetMainHUD(){ return m_SurvivorMainHUD; };

public:
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Reliable, Client)
		void ClientM();
};
