// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/MyInfo.h"
#include <Engine/DataTable.h>
#include <MyProp/Player/MyCharacterState.h>
//#include <MyProp/Player/Killer/Killer.h>
//#include <MyProp/Player/Survivor/Survivor.h>

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

	//���θ��� ������ ��������Ʈ ����
	TSubclassOf<APawn> m_Killer;
	TSubclassOf<APawn> m_Survivor;

public:
	//�÷��̾� ��
	int iPlayerCnt = 0;

public:
	const TSubclassOf<APawn> GetKiller() {
		return m_Killer;
	};
	const TSubclassOf<APawn> GetSurvivor() {
		return m_Survivor;
	};
	const FSurvivorInfo* GetSurvivorInfo(const FString& _RowName);
	const FKillerInfo* GetKillerInfo(const FString& _RowName);

	//ĳ���� ����
private:
	EPLAYER_TYPE m_SelectType;
public:
	EPLAYER_TYPE GetSelectType() { return m_SelectType; };
	void SetSelectType(EPLAYER_TYPE value) { m_SelectType = value; }

public:
	UMyGameInstance();
	~UMyGameInstance();
};