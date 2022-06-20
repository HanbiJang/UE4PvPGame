// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Kismet/GameplayStatics.h"
#include <MyProp/GameInstance/MyGameInstance.h>

//UI
#include "Components/TextBlock.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMatchingHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyMatchingHUD : public UUserWidget
{
	GENERATED_BODY()

public:
		//플레이어 인원수 나타내는 텍스트
		UTextBlock* TB_MatchingPlayer;

		//플레이어 인원수
		int CurPlayerNum;

private:
	/*virtual*/ void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

	//게임 인스턴스
	UMyGameInstance* GI;

	UFUNCTION(reliable, Server)
		void UpdatePlayerNum();


};
