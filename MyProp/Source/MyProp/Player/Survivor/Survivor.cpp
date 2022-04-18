// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor.h"

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

	//======플레이어 (초기) 데이터 설정======
	AMyCharacter* Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	const FServivorInfo* Info = nullptr;
	//데이터 테이블에서 가져오기

	SetInfo(*Info);
}
