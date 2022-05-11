// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UI
#include "MyHPBarWidget.h"
#include "MySPWidget.h"
#include "MyMachineWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

	//스테미나
	UMySPWidget* m_SPWidget;
	UMySPWidget* GetSPHUD() { return m_SPWidget; }

	//체력
	UMyHPBarWidget* m_HPWidget;
	UMyHPBarWidget* GetHPHUD() { return m_HPWidget; }

	//기계 수리
	UMyMachineWidget* m_MachineWidget;
	UMyMachineWidget* GetMachineHUD() { return m_MachineWidget; }

};
