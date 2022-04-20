// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"


const FSurvivorInfo* UMyGameInstance::GetSurvivorInfo(const FString& _RowName)
{
	FSurvivorInfo* Info = m_SurvivorTable->FindRow<FSurvivorInfo>(FName(_RowName), TEXT(""));
	return Info;
}

const FKillerInfo* UMyGameInstance::GetKillerInfo(const FString& _RowName)
{
	FKillerInfo* Info = m_KillerTable->FindRow<FKillerInfo>(FName(_RowName), TEXT(""));
	return Info;
}

UMyGameInstance::UMyGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> SurvivorTable(
		TEXT("DataTable'/Game/Blueprints/Survivor/DT_Survivor.DT_Survivor'"));
	if (SurvivorTable.Succeeded())
	{
		m_SurvivorTable = SurvivorTable.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> KillerTable(
		TEXT("DataTable'/Game/Blueprints/Killer/DT_Killer.DT_Killer'"));
	if (KillerTable.Succeeded())
	{
		m_KillerTable = KillerTable.Object;
	}
}

UMyGameInstance::~UMyGameInstance()
{
}
