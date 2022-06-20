// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/Controller/MyMatchingController.h>

#include <MyProp/GameInstance/MyGameInstance.h>

void AMyMatchingController::BeginPlay() {

	//HUD 설정하기
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_MatchingMainHUDClass = GI->GetMatchingHUDClass();
		m_MatchingMainHUD = Cast<UMyMatchingHUD>(CreateWidget(this, m_MatchingMainHUDClass));
		if (nullptr != m_MatchingMainHUD) m_MatchingMainHUD->AddToViewport();
	}

	//// InputMode 설정
	//APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameOnly mode; //UI 클릭 불가
	//Controller->SetInputMode(mode); //변경필요


}

void AMyMatchingController::PlayerTick(float DeltaTime) {

}
