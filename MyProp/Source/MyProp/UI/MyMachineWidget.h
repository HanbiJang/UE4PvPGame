// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <components/progressbar.h>
#include <components/TextBlock.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMachineWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyMachineWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UProgressBar* m_PgMachine;
	UTextBlock* m_MachineText;
	
public:
	void SetPgMachine(float _CurHPRatio);
	void SetMachineText(const FString& _Name);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
