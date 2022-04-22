// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UI
#include "UI/MyMainHUD.h"
#include "UI/MyHPBarWidget.h"
#include "UI/MySPWidget.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPropGameModeBase.generated.h"

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
	UMyMainHUD* m_MainHUD;
	UMyMainHUD* GetMainHUD() { return m_MainHUD; }

	//Character


public:
	void UpdatePlayHUD(float _CurHPRatio, float _CurMPRatio);

};
