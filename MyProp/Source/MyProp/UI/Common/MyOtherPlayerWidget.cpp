// Fill out your copyright notice in the Description page of Project Settings.

#include <MyProp/UI/Common/MyOtherPlayerWidget.h>
#include "MyOtherPlayerWidget.h"

#include <MyProp/Mode/MyPropGameModeBase.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include "Kismet/GameplayStatics.h"

void UMyOtherPlayerWidget::SetText_OtherPlayerNames(TArray<FString> nameArr)
{
	for(int i =0; i< m_OtherPlayerNameArr.Num(); i++){
		if (nullptr != m_OtherPlayerNameArr[i])
		{
			m_OtherPlayerNameArr[i]->SetText(FText::FromString(nameArr[i])); //_Name으로 FText를 생성한다
		}
	}

}

void UMyOtherPlayerWidget::SetOtherPlayerImg(int PlayerIdx, int ImgIdx)
{
	if (PlayerImg[ImgIdx])
		m_PlayerImgArr[PlayerIdx]->SetBrushFromTexture(PlayerImg[ImgIdx]);

	else {
		//GI에서 이미지 가져오기
		UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI != nullptr) {
			UTexture2D* PlayerImgAsset = Cast<UTexture2D>(GI->GetPlayerImg(ImgIdx));
			if (PlayerImgAsset) {
				PlayerImg[ImgIdx] = PlayerImgAsset;
				m_PlayerImgArr[PlayerIdx]->SetBrushFromTexture(PlayerImg[ImgIdx]);
			}
		}

	}
}

void UMyOtherPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//플레이어 이름 표시 텍스트 UI 가져오기
	m_OtherPlayerNameArr.SetNum(4);
	for (int i = 0; i < m_OtherPlayerNameArr.Num(); i++) {
		FString str = FString::Printf(TEXT("OtherPlayerName_%i"), i + 1);
		m_OtherPlayerNameArr[i] = Cast<UTextBlock>(GetWidgetFromName(*str));
	}

	//플레이어 상태등 이미지 가져오기
	m_PlayerImgArr.SetNum(4);
	for (int i = 0; i < m_PlayerImgArr.Num(); i++) {
		FString str = FString::Printf(TEXT("OtherPlayerImg_%i"), i + 1);
		m_PlayerImgArr[i] = Cast<UImage>(GetWidgetFromName(*str));
	}

	//GI에서 모든 플레이어 이미지 가져오기
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		PlayerImg = GI->GetAllPlayerImg();
	}
}

void UMyOtherPlayerWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}
