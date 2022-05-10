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

	//캐릭터 블루프린트 클래스 가져오기 (게임 인스턴스)
	ConstructorHelpers::FClassFinder<APawn>
		Killer(TEXT("Blueprint'/Game/Blueprints/Killer/BP_Killer.BP_Killer_C'"));
	if (Killer.Succeeded())
		m_Killer = Killer.Class;

	ConstructorHelpers::FClassFinder<APawn>
		Survivor(TEXT("Blueprint'/Game/Blueprints/Survivor/BP_Survivor.BP_Survivor_C'"));
	if (Survivor.Succeeded())
		m_Survivor = Survivor.Class;

	//캐릭터 UI 가져오기
	ConstructorHelpers::FClassFinder<UUserWidget>
		KillerWidget(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/Killer/BP_KillerMainHUD.BP_KillerMainHUD_C'"));
	if (KillerWidget.Succeeded())
		m_KillerWidgetClass = KillerWidget.Class;

	ConstructorHelpers::FClassFinder<UUserWidget>
		SurvivorWidget(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'"));
	if (SurvivorWidget.Succeeded())
		m_SurvivorWidgetClass = SurvivorWidget.Class;


}

UMyGameInstance::~UMyGameInstance()
{
}
