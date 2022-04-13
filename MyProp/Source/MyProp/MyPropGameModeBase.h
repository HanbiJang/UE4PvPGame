// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
};
