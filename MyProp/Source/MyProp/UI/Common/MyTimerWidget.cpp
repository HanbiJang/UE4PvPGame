// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTimerWidget.h"

#include <MyProp/Mode/MyPropGameModeBase.h>

void UMyTimerWidget::NativeConstruct() {
	Super::NativeConstruct();

	//시간 표시 텍스트 UI 가져오기
	m_TimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimeText")));

	m_MachineImgArr.SetNum(5);
	for (int i = 0; i < m_MachineImgArr.Num(); i++) {
		FString str = FString::Printf(TEXT("MachineImg_%i"), i+1);
		m_MachineImgArr[i] = Cast<UImage>(GetWidgetFromName(*str));
	}

	//이미지 가져오기 (생성자가 아니므로 이렇게 가져와야)
	UTexture2D* DoneImgAsset = LoadObject<UTexture2D>(GetWorld(),
		TEXT("Texture2D'/Game/MyImages/Machine_Done.Machine_Done'"));
	if (DoneImgAsset) {
		DoneImg = DoneImgAsset;
	}

}
void UMyTimerWidget::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}


void UMyTimerWidget::SetTimeText(const FString& _Name)
{
	if (nullptr != m_TimeText)
	{
		m_TimeText->SetText(FText::FromString(_Name)); //_Name으로 FText를 생성한다
	}
}


void UMyTimerWidget::SetMachineImge_Done(int idx) {
	m_MachineImgArr[idx]->SetBrushFromTexture(DoneImg);
}
