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

	void PosessToPawn(); //같은 카메라 pawn 에게 할당되도록 하기

public:
	virtual void PlayerTick(float DeltaTime) override;
	void BeginPlay() override;

};
