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

	//메인UI 가져오기
	//_C 포함해주기!
	ConstructorHelpers::FClassFinder<UUserWidget> MainHUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'"));
	if (MainHUD.Succeeded())
	{
		m_MainHUDClass = MainHUD.Class;
	}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//메인 UI 설정하기
	m_MainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));

	if (nullptr != m_MainHUD)
		m_MainHUD->AddToViewport();
}

void AMyPropGameModeBase::UpdatePlayHUD(float _CurHPRatio, float _CurSPRatio) {

	//체력 UI
	UMyHPBarWidget* pHPHUD = m_MainHUD->GetHPHUD();

	//스테미나 UI
	UMySPWidget* pSPHUD = m_MainHUD->GetSPHUD();

	if (pHPHUD) {
		pHPHUD->SetHP(_CurHPRatio);
		pHPHUD->SetText(TEXT("MyPlayerName"));
	}
	if (pSPHUD) {
		pSPHUD->SetSP(_CurSPRatio);
	}
}