// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API ASurvivor : public AMyCharacter
{
	GENERATED_BODY()


	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Interaction();

	void BeginPlay() override;
};
