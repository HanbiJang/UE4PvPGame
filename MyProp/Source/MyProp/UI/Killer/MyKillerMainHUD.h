// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <components/progressbar.h>
#include <components/TextBlock.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyKillerMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyKillerMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar* m_QSkillPg;
	UProgressBar* m_ESkillPg;
	UProgressBar* m_ClickSkillPg;

	UTextBlock* m_QSkillText;
	UTextBlock* m_ESkillText;
	UTextBlock* m_ClickSkillText;

public:
	//À§Á¬ ÁÖ±â
	UProgressBar* GetQSkillPg() { return m_QSkillPg; }
	UProgressBar* GetESkillPg(){ return m_ESkillPg; }
	UProgressBar* GetClickSkillPg(){ return m_ClickSkillPg; }
	UTextBlock* GetQSkillText(){ return m_QSkillText; }
	UTextBlock* GetESkillText(){ return m_ESkillText; }
	UTextBlock* GetClickSkillText(){ return m_ClickSkillText; }

	//À§Á¬ °ª¼³Á¤
	void SetQSkillPg(float _CurTime);
	void SetESkillPg(float _CurTime);
	void SetClickSkillPg(float _CurTime);

	void SetText_QSkill(const FString& _CurTime);
	void SetText_ESkill(const FString& _CurTime);
	void SetText_ClickSkill(const FString& _CurTime);


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
