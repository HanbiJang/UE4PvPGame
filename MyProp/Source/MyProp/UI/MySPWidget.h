// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <components/progressbar.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MySPWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMySPWidget : public UUserWidget
{
	GENERATED_BODY()



private:
	UProgressBar* m_SPBar;

public:
	void SetSP(float _CurSPRatio);


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
