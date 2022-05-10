// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Player/Common/MyInfo.h>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyEffectManager.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyEffectManager : public UObject
{
	GENERATED_BODY()

public:
	static UMyEffectManager* m_This;
	static UWorld* m_World;
	

public:
	static UMyEffectManager* GetInst(UWorld* _world);
	static void Destroy();


public:
	void CreateEffect(EKillerEffect _type, FTransform _trans, ULevel* _Level);

private:
	UMyEffectManager();

};
