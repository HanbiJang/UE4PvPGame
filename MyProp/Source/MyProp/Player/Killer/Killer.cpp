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

void AKiller::BeginPlay() {
	Super::BeginPlay();

	//======플레이어 (초기) 데이터 설정======
	AMyCharacter* Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	const FKillerInfo* Info = nullptr;
	//데이터 테이블에서 가져오기

	SetInfo(*Info);
}
