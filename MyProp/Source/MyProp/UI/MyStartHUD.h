// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"//게임종료
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStartHUD.generated.h"

class UMyStartGameWidget;

/**
 * 
 */
UCLASS()
class MYPROP_API UMyStartHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	/*virtual*/ void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

	//플레이 버튼 : 게임 시작 패널 띄우기
	UButton* m_PlayBtn;
	UMyStartGameWidget* m_StartGameWidget;

	//플레이어 이름 설정
	//ET_MyPlayerName
	UEditableTextBox* ET_MyPlayerName;

	//나가기 버튼 : 게임 종료하기
	UButton* m_ExitBtn;

public:
	UButton* GetPlayBtn() { return m_PlayBtn;}
	UButton* GetExitBtn() { return m_ExitBtn; }
	UMyStartGameWidget* GetStartGameWidget() { return m_StartGameWidget; }
	UEditableTextBox* GetET_MyPlayerName() { return ET_MyPlayerName; }
	
	//플레이 버튼 : 게임 시작 패널 띄우기
	UFUNCTION()
		void ShowStartGameUI();

	//나가기 버튼 : 게임 종료하기
	UFUNCTION()
		void ExitGame();

	//홈화면 돌아가기 버튼
	UFUNCTION()
		void ShowHomeUI();

	//글자 설정 (바인드할 함수)
	UFUNCTION()
		void OnMyPlayerNameTextChange(const FText& Text);

};
