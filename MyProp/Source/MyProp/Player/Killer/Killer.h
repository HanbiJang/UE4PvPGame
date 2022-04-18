// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine/DataTable.h>
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

public:
		AKiller();

private:
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Attack();
	void BeginPlay() override;

};
