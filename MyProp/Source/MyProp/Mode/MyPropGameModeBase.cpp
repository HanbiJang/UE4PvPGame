// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {

	//생존자 메인UI 가져오기
	ConstructorHelpers::FClassFinder<UUserWidget> SurvivorMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'")); //_C 포함해주기!
	//살인마 메인UI
	ConstructorHelpers::FClassFinder<UUserWidget> KillerMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/Killer/BP_KillerMainHUD.BP_KillerMainHUD_C'")); //_C 포함해주기!
	
	DefaultPawnClass = nullptr;

	//선택 상태 가져오기
	EPLAYER_TYPE m_SelectType;
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SelectType = GI->GetSelectType();

		//[1] 생존자 코드
		switch (m_SelectType)
		{
		case EPLAYER_TYPE::KILLER:
			DefaultPawnClass = GI->GetKiller();
			if (KillerMainHUD.Succeeded()) m_MainHUDClass = KillerMainHUD.Class; //UI
			break;
		case EPLAYER_TYPE::SURVIVOR:
			DefaultPawnClass = GI->GetSurvivor();
			if (SurvivorMainHUD.Succeeded()) m_MainHUDClass = SurvivorMainHUD.Class; //UI
			break;
		}
	}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//메인 UI 설정하기
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			m_MainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
			if (nullptr != m_MainHUD) m_MainHUD->AddToViewport();
		}
		else {
			m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
			if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
		}
		
	}

	// InputMode 설정
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI mode;

	Controller->SetInputMode(mode);
	Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.

}

void AMyPropGameModeBase::UpdatePlayHUD(float _CurHPRatio, float _CurSPRatio) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			//체력 UI
			UMyHPBarWidget* pHPHUD = m_MainHUD->GetHPHUD();

			//스테미나 UI
			UMySPWidget* pSPHUD = m_MainHUD->GetSPHUD();

			if (pHPHUD) {
				pHPHUD->SetHP(_CurHPRatio);
				pHPHUD->SetText(TEXT("Survivor 1"));
			}
			if (pSPHUD) {
				pSPHUD->SetSP(_CurSPRatio);
			}
		}
		else {

		}
	}
}