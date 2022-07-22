// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyMatchingModeBase.generated.h"

/**
 * 
 */
class UMyGameInstance;
UCLASS()
class MYPROP_API AMyMatchingModeBase : public AGameModeBase
{
	GENERATED_BODY()

		AMyMatchingModeBase();

	//컨트롤러에서 UI 띄우고, 모드에서는 인원수를 체크해서 다음 게임맵으로 이어준다

private:
	void PostLogin(APlayerController* NewPlayer) override; //로그인이 성공한뒤에 호출

	int playerNum = 0;

	void Tick(float DeltaTime) override;


		void UpdatePlayerNum();

	UMyGameInstance* GI;

	void BeginPlay();

	FTimerHandle FGameStartTimer;

	void GoGameMap();
	
};
