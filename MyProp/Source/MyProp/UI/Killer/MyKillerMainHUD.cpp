// Fill out your copyright notice in the Description page of Project Settings.


#include "MyKillerMainHUD.h"

void UMyKillerMainHUD::NativeConstruct() {
	Super::NativeConstruct();

	//UI 가져오기
	m_QSkillPg = Cast<UProgressBar>(GetWidgetFromName(TEXT("QSkillPg")));
	m_ESkillPg = Cast<UProgressBar>(GetWidgetFromName(TEXT("ESkillPg")));
	m_ClickSkillPg = Cast<UProgressBar>(GetWidgetFromName(TEXT("ClickSkillPg")));

	m_QSkillText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QSkillText")));
	m_ESkillText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ESkillText")));
	m_ClickSkillText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ClickSkillText")));

	//UI 초기 세팅
	SetQSkillPg(0);
	SetESkillPg(0);
	SetClickSkillPg(0);
	m_QSkillText->SetVisibility(ESlateVisibility::Hidden);
	m_ESkillText->SetVisibility(ESlateVisibility::Hidden);
	m_ClickSkillText->SetVisibility(ESlateVisibility::Hidden);

}
void UMyKillerMainHUD::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}

//===================================================================================

void UMyKillerMainHUD::SetQSkillPg(float _CurTime)
{
	if (nullptr != m_QSkillPg)
	{
		m_QSkillPg->SetPercent(_CurTime);
	}
}

void UMyKillerMainHUD::SetESkillPg(float _CurTime)
{
	if (nullptr != m_ESkillPg)
	{
		m_ESkillPg->SetPercent(_CurTime);
	}
}

void UMyKillerMainHUD::SetClickSkillPg(float _CurTime)
{
	if (nullptr != m_ClickSkillPg)
	{
		m_ClickSkillPg->SetPercent(_CurTime);
	}
}

void UMyKillerMainHUD::SetText_QSkill(const FString& _CurTime)
{
	if (nullptr != m_QSkillText)
	{
		m_QSkillText->SetText(FText::FromString(_CurTime)); //_Name으로 FText를 생성한다
	}
}

void UMyKillerMainHUD::SetText_ESkill(const FString& _CurTime)
{
	if (nullptr != m_ESkillText)
	{
		m_ESkillText->SetText(FText::FromString(_CurTime)); //_Name으로 FText를 생성한다
	}
}

void UMyKillerMainHUD::SetText_ClickSkill(const FString& _CurTime)
{
	if (nullptr != m_ClickSkillText)
	{
		m_ClickSkillText->SetText(FText::FromString(_CurTime)); //_Name으로 FText를 생성한다
	}
}

