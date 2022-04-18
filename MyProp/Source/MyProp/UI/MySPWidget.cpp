// Fill out your copyright notice in the Description page of Project Settings.


#include "MySPWidget.h"
void UMySPWidget::NativeConstruct() {
	Super::NativeConstruct();

	//스테미너 UI 가져오기
	m_SPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SPBar")));
}
void UMySPWidget::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}

void UMySPWidget::SetSP(float _fRatio)
{
	if (nullptr != m_SPBar)
	{
		m_SPBar->SetPercent(_fRatio);
	}
}