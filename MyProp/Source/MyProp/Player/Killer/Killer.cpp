// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 왼클릭 - 공격 키
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);

}

void AKiller::Attack()
{
	//공격 모션
}
