// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/Controller/MyMatchingController.h>

#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/UI/Common/MyMatchingHUD.h>

//void AMyMatchingController::BeginPlay() {
//
//	//HUD 설정하기
//	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (GI != nullptr) {
//		m_MatchingMainHUDClass = GI->GetMatchingHUDClass();
//		m_MatchingMainHUD = Cast<UMyMatchingHUD>(CreateWidget(this, m_MatchingMainHUDClass));
//		if (nullptr != m_MatchingMainHUD) m_MatchingMainHUD->AddToViewport();
//	}
//
//	//// InputMode 설정
//	//APlayerController* Controller = GetWorld()->GetFirstPlayerController();
//	//FInputModeGameOnly mode; //UI 클릭 불가
//	//Controller->SetInputMode(mode); //변경필요
//
//	PosessToPawn();
//
//}
//
//void AMyMatchingController::PosessToPawn() //같은 카메라 pawn 에게 할당되도록 하기
//{
//
//
//	for (TObjectIterator<APawn> e; e; ++e) 
//	{
//		if (e->Tags[0].ToString().Equals("MatchingCamera")) 
//		{
//			//플레이어들이 월드 상의 같은 카메라 액터에게 Posess 설정하기
//			GetWorld()->GetFirstPlayerController()->Possess(*e);
//			break;
//		}
//	}
//}
//
//void AMyMatchingController::UpdatePlayerNum() //UI 상에서 플레이어 수를 반영하기
//{
//
//}
//
//void AMyMatchingController::PlayerTick(float DeltaTime) {
//
//	UpdatePlayerNum(); //플레이어 수를 반영하기
//
//}
