// Fill out your copyright notice in the Description page of Project Settings.

#include "MyStartHUD.h"
#include "MyStartGameWidget.h"
//#include <MyProp/Mode/MyStartModeBase.h>
#include <MyProp/GameInstance/MyGameInstance.h>

void UMyStartHUD::NativeConstruct() {
	Super::NativeConstruct();

	//이름으로 위젯 가져오기
	m_PlayBtn = Cast<UButton>(GetWidgetFromName(TEXT("PlayBtn")));
	m_ExitBtn = Cast<UButton>(GetWidgetFromName(TEXT("ExitBtn")));
	m_StartGameWidget = Cast<UMyStartGameWidget>(GetWidgetFromName(TEXT("StartGamewidget")));
	ET_MyPlayerName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ET_MyPlayerName")));

	//위젯에 기능 할당
	if (IsValid(m_PlayBtn))
	{
		m_PlayBtn->OnClicked.AddDynamic(this, &UMyStartHUD::ShowStartGameUI);
	}

	if (IsValid(m_ExitBtn))
	{
		m_ExitBtn->OnClicked.AddDynamic(this, &UMyStartHUD::ExitGame);
	}

	if (IsValid(m_StartGameWidget))
	{
		m_StartGameWidget->GetHomeBtn()->OnClicked.AddDynamic(this, &UMyStartHUD::ShowHomeUI);
	}

	if(IsValid(ET_MyPlayerName)){ //텍스트 박스에 이름 넣기
		ET_MyPlayerName->OnTextChanged.AddDynamic(this, &UMyStartHUD::OnMyPlayerNameTextChange);
	}

}

void UMyStartHUD::NativeTick(const FGeometry& Geometry, float DT){
	Super::NativeTick(Geometry, DT);
}

void UMyStartHUD::ShowStartGameUI() {
	m_StartGameWidget->SetVisibility(ESlateVisibility::Visible);

	//메인 버튼 숨기기
	m_PlayBtn->SetVisibility(ESlateVisibility::Hidden);
	m_ExitBtn->SetVisibility(ESlateVisibility::Hidden);
}

void UMyStartHUD::ExitGame() {
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		EQuitPreference::Quit, false);
	UE_LOG(LogTemp, Log, TEXT("ExitGame"));
}

void UMyStartHUD::ShowHomeUI() {
	//'게임 찾기' 화면 끄기
	GetStartGameWidget()->SetVisibility(ESlateVisibility::Hidden);

	//메인 버튼 보이기
	m_PlayBtn->SetVisibility(ESlateVisibility::Visible);
	m_ExitBtn->SetVisibility(ESlateVisibility::Visible);
}

void UMyStartHUD::OnMyPlayerNameTextChange(const FText& Text){
	//텍스트를 게임 인스턴스에 정보 넘기기
	UMyGameInstance* GI = Cast<UMyGameInstance>( UGameplayStatics::GetGameInstance(GetWorld()) );
	if (GI) {
		GI->MyPlayerName = Text.ToString();
	}
	//else {
	//	GI->MyPlayerName = "DefaultName";
	//}
}
