// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMainHUD.h"

//생성 시 실행
void UMyMainHUD::NativeConstruct() {
	Super::NativeConstruct();

	//UI 가져오기
	//스테미나
	m_SPWidget = Cast<UMySPWidget>(GetWidgetFromName(TEXT("BP_MySP")));
	//체력 
	m_HPWidget = Cast<UMyHPBarWidget>(GetWidgetFromName(TEXT("BP_MyHP")));

}


//틱마다 실행됨
void UMyMainHUD::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);

}
