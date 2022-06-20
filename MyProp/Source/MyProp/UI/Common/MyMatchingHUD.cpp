// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/UI/Common/MyMatchingHUD.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/Mode/MyMatchingModeBase.h>

void UMyMatchingHUD::NativeConstruct() {
	
	//UI가져오기
	TB_MatchingPlayer = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_MatchingPlayer")));
	
	//게임 인스턴스 가져오기
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI) {
		CurPlayerNum = GI->curPlayerNum;
	}

}
void UMyMatchingHUD::NativeTick(const FGeometry& Geometry, float DT) {

	UpdatePlayerNum();

}

void UMyMatchingHUD::UpdatePlayerNum_Implementation() {
	//인원수를 게임인스턴스의 플레이어 수로 업데이트하기
	AMyMatchingModeBase* m_mode = Cast<AMyMatchingModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//CurPlayerNum = GI->curPlayerNum;
	CurPlayerNum = m_mode->GetNumPlayers();
	
	//텍스트 업데이트
	FString str = FString::Printf(TEXT("%d / %d"), CurPlayerNum, GI->maxPlayer);
	//FString str = FString::Printf(TEXT("%d명 ... %d명"), (int)CurPlayerNum, (int)GI->maxPlayer); //한국어 불가
	TB_MatchingPlayer->SetText(FText::FromString(str));
}