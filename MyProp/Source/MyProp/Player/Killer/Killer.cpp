// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"

AKiller::AKiller() {

	//======플레이어 (초기) 데이터 설정======
	const FKillerInfo* Info = nullptr;

	//데이터 테이블 블루프린트에서 가져오기
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/Blueprints/DT_Killer.DT_Killer'"));
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;

		if (m_PlayerTable) {
			Info = m_PlayerTable->FindRow<FKillerInfo>(FName(TEXT("Killer1")), TEXT(""));
			SetInfo(*Info);
		}

	}

}

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
}
