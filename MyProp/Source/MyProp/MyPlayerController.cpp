// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/MyGameInstance.h>

void AMyPlayerController::DrawHUD_Client_Implementation() {
	//UI
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();

		m_SurvivorMainHUD = Cast<UMyMainHUD>(CreateWidget(this, m_SurvivorMainHUDClass));
		m_SurvivorMainHUD->AddToViewport();
	}

	// InputMode 설정
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI mode;
	Controller->SetInputMode(mode); //변경필요
	Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.
}


