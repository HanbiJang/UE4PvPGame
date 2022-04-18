// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHPBarWidget.h"

void UMyHPBarWidget::NativeConstruct() {
	Super::NativeConstruct();

	//체력,이름 UI 가져오기
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
}
void UMyHPBarWidget::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}

void UMyHPBarWidget::SetHP(float _fRatio)
{
	if (nullptr != m_HPBar)
	{
		m_HPBar->SetPercent(_fRatio);
	}
}

void UMyHPBarWidget::SetText(const FString& _Name)
{
	if (nullptr != m_PlayerName)
	{
		m_PlayerName->SetText(FText::FromString(_Name)); //_Name으로 FText를 생성한다
	}
}

