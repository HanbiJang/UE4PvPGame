// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraActor.h"
#include <MyProp/UI/MyStartHUD.h>
#include <MyProp/UI/MyStartGameWidget.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyStartModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API AMyStartModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	TSubclassOf<UMyStartHUD>	m_HUDClass;
	UMyStartHUD* m_HUD;
	ACameraActor* m_Cam;

public:
	UMyStartHUD* GetHUD() { return m_HUD; }

public:
	AMyStartModeBase();

public:
	virtual void BeginPlay() override;
	
};
