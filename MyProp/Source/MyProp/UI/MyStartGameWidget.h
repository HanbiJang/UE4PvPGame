// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Player/Common/MyCharacterState.h>
#include "Components/Button.h"
#include <MyProp/GameInstance/MyGameInstance.h>

//매칭
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStartGameWidget.generated.h"

class UMyStartHUD;
class USelectCharacter;

/**
 * 
 */
UCLASS()
class MYPROP_API UMyStartGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	/*virtual*/ void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

	//홈 버튼
	//생존자 버튼
	//살인마 버튼
	//아무거나 버튼
	//게임시작 버튼
	UButton* m_HomeBtn;
	UButton* m_RandomBtn;
	UButton* m_KillerBtn;
	UButton* m_SurvivorBtn;
	UButton* m_StartGameBtn;

public:

	//홈 버튼 : 홈화면으로 되돌아가기
	UButton* GetHomeBtn() { return m_HomeBtn; }
	UButton* GetRandomBtn() { return m_RandomBtn; }
	UButton* GetKillerBtn() { return m_KillerBtn; }
	UButton* GetSurvivorBtn() { return m_SurvivorBtn; }
	UButton* GetStartGameBtn() { return m_StartGameBtn; }

public:
	//버튼 당 기능
	UFUNCTION()
		void RandomBtnPressed();

	UFUNCTION()
		void KillerBtnPressed();

	UFUNCTION()
		void SurvivorBtnPressed();

	UFUNCTION()
		void StartGameBtnPressed();


public:
	//매칭 시스템 ===================================
	void CreateServer();
	void FindServer();
	void Init();

	void MySession();
	virtual void OnCreateSessionComplete(FName ServerName, bool Succeded); //세션 생성이 끝났을때 호출하는 함수  
	IOnlineSessionPtr SessionInterface;
};
