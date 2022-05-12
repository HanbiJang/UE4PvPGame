// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <components/progressbar.h>
#include <components/TextBlock.h>
#include "Components/Image.h"
#include "Engine/Texture2D.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTimerWidget.generated.h"

class AMyPropGameModeBase;
/**
 * 
 */
UCLASS()
class MYPROP_API UMyTimerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UTextBlock* m_TimeText;
	TArray<UImage*> m_MachineImgArr;

	//이미지 변경
	UTexture2D* DoneImg;

public:
	void SetTimeText(const FString& _Name);

	void SetMachineImge_Done(int idx); //왼쪽부터 하나씩 변경

public:
	/*virtual */void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
