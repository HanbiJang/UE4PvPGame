// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor.h"

ASurvivor::ASurvivor() {
	//======플레이어 (초기) 데이터 설정======
	const FServivorInfo* Info = nullptr;

	//데이터 테이블 블루프린트에서 가져오기
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/Blueprints/DT_Survivor.DT_Survivor'"));
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;
	}
	Info = m_PlayerTable->FindRow<FServivorInfo>(FName(TEXT("Survivor1")), TEXT(""));

	SetInfo(*Info);
}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);
}

void ASurvivor::Interaction()
{
	//발전기, 문 등과 상호작용
}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();
}
