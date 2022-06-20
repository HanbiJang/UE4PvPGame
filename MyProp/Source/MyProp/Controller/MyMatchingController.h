// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/UI/Common/MyMatchingHUD.h>

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyMatchingController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API AMyMatchingController : public APlayerController
{
	GENERATED_BODY()
	
	//HUD
	TSubclassOf<UUserWidget> m_MatchingMainHUDClass;
	UMyMatchingHUD* m_MatchingMainHUD;
	UMyMatchingHUD* GetMainHUD() { return m_MatchingMainHUD; };


public:
	virtual void PlayerTick(float DeltaTime) override;
	void BeginPlay() override;
};
