// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"

AMyPropGameModeBase::AMyPropGameModeBase() {

	//캐릭터 블루프린트 클래스 가져오기
	//프로토타입 더미 블루프린트
		//Blueprint'/Game/Blueprints/BP_Survivor.BP_Survivor'

	ConstructorHelpers::FClassFinder<APawn>
		MainCharacterClass(TEXT("Blueprint'/Game/Blueprints/BP_Survivor.BP_Survivor_C'"));
	if (MainCharacterClass.Succeeded())
	{
		DefaultPawnClass = MainCharacterClass.Class;
	}

}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}
