// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyMatchingController.generated.h"

/**
 * 
 */
class UMyMatchingHUD;
UCLASS()
class MYPROP_API AMyMatchingController : public APlayerController
{
	GENERATED_BODY()
	
//	//HUD
//	TSubclassOf<UUserWidget> m_MatchingMainHUDClass;
//	UMyMatchingHUD* m_MatchingMainHUD;
//	UMyMatchingHUD* GetMainHUD() { return m_MatchingMainHUD; };
//
//	void PosessToPawn(); //같은 카메라 pawn 에게 할당되도록 하기
//	void UpdatePlayerNum(); //UI 상에서 플레이어 수를 반영하기
//
//	int ID;
//
//public:
//	void SetID(int i) { ID = i; }
//	int GetID() { return ID; }

//public:
//	virtual void PlayerTick(float DeltaTime) override;
//	void BeginPlay() override;

};
