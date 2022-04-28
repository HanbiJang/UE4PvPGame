// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStartModeBase.h"

AMyStartModeBase::AMyStartModeBase() {
	ConstructorHelpers::FClassFinder<UUserWidget> HUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/StartUI/BP_MyStartHUD.BP_MyStartHUD_C'"));
	if (HUD.Succeeded())
	{
		m_HUDClass = HUD.Class;
	}

	DefaultPawnClass = nullptr;
}

void AMyStartModeBase::BeginPlay() {
	Super::BeginPlay();

	// HUD 화면에 등록
	m_HUD = Cast<UMyStartHUD>(CreateWidget(GetWorld(), m_HUDClass));

	if (nullptr == m_HUD)
		return;

	m_HUD->AddToViewport();
	m_HUD->GetStartGameWidget()->SetVisibility(ESlateVisibility::Hidden); // 게임 시작 - 화면에서 안보이게 하기
	
	// InputMode 설정
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();		
	FInputModeGameAndUI mode;

	Controller->SetInputMode(mode);
	Controller->bShowMouseCursor = true; // 언제나 마우스 커서가 보이게 한다.
	
}