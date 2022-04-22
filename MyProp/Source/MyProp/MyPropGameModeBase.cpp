// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include "MyGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	//캐릭터 블루프린트 클래스 가져오기

	//[1] 생존자 코드
	ConstructorHelpers::FClassFinder<APawn>
		Survivor(TEXT("Blueprint'/Game/Blueprints/Survivor/BP_Survivor.BP_Survivor_C'"));
	if (Survivor.Succeeded())
	{
		//DefaultPawnClass = Survivor.Class;
	}

	//[2] 살인마 코드
	ConstructorHelpers::FClassFinder<APawn>
		Killer(TEXT("Blueprint'/Game/Blueprints/Killer/BP_Killer.BP_Killer_C'"));
	if (Killer.Succeeded())
	{
		DefaultPawnClass = Killer.Class;
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

	//======플레이어 (초기) 데이터 설정======
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FString TypeName_ = typeid(DefaultPawnClass).name();
	UE_LOG(LogTemp, Log, TEXT("%s"), *TypeName_);

	//[1] 생존자
	//if (GI != nullptr) {
	//	ASurvivor* Character = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//	Character->SetInfo(*(GI->GetSurvivorInfo(TEXT("Survivor1"))));
	//}

	//======플레이어 (초기) 데이터 설정======
	//[2] 킬러
	if (GI != nullptr) {
		AKiller* Character = Cast<AKiller>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Character->SetInfo(*(GI->GetKillerInfo(TEXT("Killer1"))));
	}


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