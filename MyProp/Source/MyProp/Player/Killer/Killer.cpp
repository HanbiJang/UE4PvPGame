// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"

AKiller::AKiller() {
}

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 왼클릭 - 공격 키
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);
	// Q키 - 범위 공격
	PlayerInputComponent->BindAction(TEXT("Q"), EInputEvent::IE_Pressed, this, &AKiller::RangeAttack);
}

void AKiller::Attack()
{
	//공격 모션
	ChangeState(EPLAYER_STATE::ATTACK);
}

void AKiller::BeginPlay() {
	Super::BeginPlay();
}

void AKiller::RangeAttack() {
	ChangeState(EPLAYER_STATE::RANGEATTACK);
}