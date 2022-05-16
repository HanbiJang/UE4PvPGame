// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <components/TextBlock.h>
#include "Components/Image.h"
#include "Engine/Texture2D.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyOtherPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyOtherPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<UTextBlock*> m_OtherPlayerNameArr;

public:
	void SetText_OtherPlayerNames(TArray<FString> nameArr);
	TArray<UImage*> m_PlayerImgArr;

	//이미지 변경
	TArray<UTexture2D*> PlayerImg;

	void SetOtherPlayerImg(int PlayerIdx, int ImgIdx);

public:
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
	
};
