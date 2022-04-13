// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Killer.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API AKiller : public AMyCharacter
{
	GENERATED_BODY()
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Attack();
};
