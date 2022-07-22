// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/UI/Common/MyMatchingHUD.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/Mode/MyMatchingModeBase.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/Controller/MyPlayerController.h>

void UMyMatchingHUD::NativeConstruct() {
	
	//UI가져오기
	TB_MatchingPlayer = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_MatchingPlayer")));
	
	//게임 인스턴스 가져오기
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


}
void UMyMatchingHUD::NativeTick(const FGeometry& Geometry, float DT) {

	AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc) 
	{
		UpdatePlayerNum_Server(pc->iPlayerCnt);
	}
	

}

void UMyMatchingHUD::UpdatePlayerNum_Server_Implementation(int num) {
	//인원수를 게임인스턴스의 플레이어 수로 업데이트하기
	
	//텍스트 업데이트
	if (GI) {
		FString str = FString::Printf(TEXT("%d / %d"), num, GI->maxPlayer);
		//FString str = FString::Printf(TEXT("%d명 ... %d명"), (int)CurPlayerNum, (int)GI->maxPlayer); //한국어 불가
		TB_MatchingPlayer->SetText(FText::FromString(str));
	}
}