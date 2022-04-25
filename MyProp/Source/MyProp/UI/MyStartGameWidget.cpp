// Fill out your copyright notice in the Description page of Project Settings.

#include "MyStartGameWidget.h"
#include "MyStartHUD.h"
#include "../Player/MyCharacterState.h"

#include <MyProp/UI/Function/MySelectCharacter.h>

void UMyStartGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//이름으로 위젯 가져오기
	m_HomeBtn = Cast<UButton>(GetWidgetFromName(TEXT("HomeBtn")));	
	m_RandomBtn = Cast<UButton>(GetWidgetFromName(TEXT("RandomBtn")));
	m_KillerBtn = Cast<UButton>(GetWidgetFromName(TEXT("SurvivorBtn")));
	m_SurvivorBtn = Cast<UButton>(GetWidgetFromName(TEXT("KillerBtn")));
	m_StartGameBtn = Cast<UButton>(GetWidgetFromName(TEXT("StartGameBtn")));

	//위젯에 기능 할당
	if (IsValid(m_RandomBtn) && IsValid(m_KillerBtn) &&
		IsValid(m_SurvivorBtn) && IsValid(m_StartGameBtn))
	{
		m_RandomBtn->OnClicked.AddDynamic(this, &UMyStartGameWidget::RandomBtnPressed);
		m_KillerBtn->OnClicked.AddDynamic(this, &UMyStartGameWidget::KillerBtnPressed);
		m_SurvivorBtn->OnClicked.AddDynamic(this, &UMyStartGameWidget::SurvivorBtnPressed);
		m_StartGameBtn->OnClicked.AddDynamic(this, &UMyStartGameWidget::StartGameBtnPressed);
	}
}

void UMyStartGameWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}