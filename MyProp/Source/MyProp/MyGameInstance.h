// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/MyInfo.h"
#include <Engine/DataTable.h>

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UDataTable* m_SurvivorTable;
	UDataTable* m_KillerTable;

public:
	const FSurvivorInfo* GetSurvivorInfo(const FString& _RowName);
	const FKillerInfo* GetKillerInfo(const FString& _RowName);

public:
	UMyGameInstance();
	~UMyGameInstance();
};
