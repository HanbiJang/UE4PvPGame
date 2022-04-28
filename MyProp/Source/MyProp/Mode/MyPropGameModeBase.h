// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UI
#include <MyProp/UI/MyMainHUD.h>
#include <MyProp/UI/MyHPBarWidget.h>
#include <MyProp/UI/MySPWidget.h>

#include <MyProp/UI/MyStartGameWidget.h>


#include <MyProp/UI/Killer/MyKillerMainHUD.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPropGameModeBase.generated.h"

class AKiller;
class ASurvivor;

/**
 * 
 */
UCLASS()
class MYPROP_API AMyPropGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	//MyCharacter에게 폰 빙의 시키기
	AMyPropGameModeBase();

	void BeginPlay();

	//UI
	TSubclassOf<UUserWidget>	m_MainHUDClass;

	TSubclassOf<UUserWidget>	m_KillerMainHUDClass;
	TSubclassOf<UUserWidget>	m_SurvivorMainHUDClass;

	UMyMainHUD* m_SurvivorMainHUD;
	//UMyMainHUD* GetMainHUD() { return m_SurvivorMainHUD; }

	UMyKillerMainHUD* m_KillerMainHUD;
	UMyKillerMainHUD* GetKillerMainHUD() { return m_KillerMainHUD; }

	//플레이어들
	int maxPlayerNum = 2;
	AKiller* pKillerPlayer;
	ASurvivor* pSurvivorPlayer1;

	FVector vKillerSpawnLocation = FVector(240,570,200);
	FVector vSurvivor1SpawnLocation = FVector(240,940,200);


public:
	void UpdatePlayHUD(float _CurHPRatio, float _CurMPRatio);

private:
	virtual void PostLogin(APlayerController* NewPlayer) override; //로그인이 성공한뒤에 호출됨
};
