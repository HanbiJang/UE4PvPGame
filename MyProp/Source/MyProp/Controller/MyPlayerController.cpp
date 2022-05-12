// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/UI/Common/MyTimerWidget.h>

#include <MyProp/Mode/MyPropGameModeBase.h>
#include <MyProp/Machine/MyMachine.h>

AMyPlayerController::AMyPlayerController():
	DoneMachineNum(0),
	IsAllMachineRepaired(false)
{

}

void AMyPlayerController::DrawHUD_Client_Implementation() 
{
	//UI
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();
		m_SurvivorMainHUD = Cast<UMyMainHUD>(CreateWidget(this, m_SurvivorMainHUDClass));
		if (nullptr != m_SurvivorMainHUD) m_SurvivorMainHUD->AddToViewport();

		//발전기 UI 숨기기
		m_SurvivorMainHUD->GetMachineHUD()->SetVisibility(ESlateVisibility::Hidden);
	}

	// InputMode 설정
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly mode; //UI 클릭 불가
	Controller->SetInputMode(mode); //변경필요

}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	
	if (DoneMachineNum >= 5) { //발전기 수리가 5개됐으면
		IsAllMachineRepaired = true; //모든 발전기가 수리되었다는 것을 알리자
	}
}
void AMyPlayerController::DrawHUD_Server_Implementation() {
	//UI 설정
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_KillerMainHUDClass = GI->GetKillerWidgetClass();
		m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
		if (nullptr != m_KillerMainHUD) { 
			m_KillerMainHUD->AddToViewport(); 
			
			//UI초기화 (처음에 스킬 사용 가능)
			//m_KillerMainHUD->GetClickSkillText()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// InputMode 설정
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameAndUI mode;
	FInputModeGameOnly mode; //UI 클릭 불가
	Controller->SetInputMode(mode);
	//Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.
}


void AMyPlayerController::UpdatePlayHUD_Killer_Implementation(float _CurQTimeRatio, float _CurETimeRatio, float _CurRCTimeRatio
, float _CurQTime, float _CurETime, float _CurRCTime) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {

		if (m_KillerMainHUD != nullptr) {
			//프로그래스바 설정
			m_KillerMainHUD->SetQSkillPg(_CurQTimeRatio);
			m_KillerMainHUD->SetESkillPg(_CurETimeRatio);
			m_KillerMainHUD->SetClickSkillPg(_CurRCTimeRatio);

			//[1]Q
			if (_CurQTime < 0.05f) {
				if(m_KillerMainHUD->GetQSkillText()->Visibility == ESlateVisibility::Visible)
					m_KillerMainHUD->GetQSkillText()->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
		
				FString _QTime = FString::Printf(TEXT("%d"), (int)_CurQTime);
				m_KillerMainHUD->SetText_QSkill(_QTime);
				if (m_KillerMainHUD->GetQSkillText()->Visibility == ESlateVisibility::Hidden) {
					m_KillerMainHUD->GetQSkillText()->SetVisibility(ESlateVisibility::Visible);
				}
			}
			//[2]E
			if (_CurETime < 0.05f) {
				if (m_KillerMainHUD->GetESkillText()->Visibility == ESlateVisibility::Visible)
					m_KillerMainHUD->GetESkillText()->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				FString _ETime = FString::Printf(TEXT("%d"), (int)_CurETime);
				m_KillerMainHUD->SetText_ESkill(_ETime);
				if (m_KillerMainHUD->GetESkillText()->Visibility == ESlateVisibility::Hidden) {
					m_KillerMainHUD->GetESkillText()->SetVisibility(ESlateVisibility::Visible);
				}

			}
			//[3]RC
			if (_CurRCTime < 0.05f) {
				if (m_KillerMainHUD->GetClickSkillText()->Visibility == ESlateVisibility::Visible)
					m_KillerMainHUD->GetClickSkillText()->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				FString _RCTime = FString::Printf(TEXT("%d"), (int)_CurRCTime);
				m_KillerMainHUD->SetText_ClickSkill(_RCTime);
				if (m_KillerMainHUD->GetClickSkillText()->Visibility == ESlateVisibility::Hidden) {
					m_KillerMainHUD->GetClickSkillText()->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}

}

void AMyPlayerController::UpdatePlayHUD_Survivor_Implementation(
	float _CurHPRatio,
	float _CurSPRatio,
	float _CurHP, 
	float _MaxHP) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			//체력 UI
			UMyHPBarWidget* pHPHUD = m_SurvivorMainHUD->GetHPHUD();

			//스테미나 UI
			UMySPWidget* pSPHUD = m_SurvivorMainHUD->GetSPHUD();

			if (pHPHUD) {
				pHPHUD->SetHP(_CurHPRatio);
				pHPHUD->SetText_PlayerName(TEXT("Survivor 1"));

				//HP SP
				FString hp = FString::Printf(TEXT("%d"), (int)_CurHP);
				FString maxhp = FString::Printf(TEXT("%d"), (int)_MaxHP);

				pHPHUD->SetText_PlayerName(TEXT("Survivor1"));
				pHPHUD->SetText_CurHP(hp);
				pHPHUD->SetText_MaxHP(maxhp);
			}
			if (pSPHUD) {
				pSPHUD->SetSP(_CurSPRatio);
			}

		}
	}

}

void AMyPlayerController::UpdateMachineHUD_Survivor_Implementation(
	float _CurMachineRatio
	) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {

			//발전기 UI
			UMyMachineWidget* pMachineHUD = m_SurvivorMainHUD->GetMachineHUD();

			//기계 수리
			if (pMachineHUD) {
				pMachineHUD->SetPgMachine(_CurMachineRatio);
			}

		}
	}

}


