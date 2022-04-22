// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Killer.generated.h"

class ASurvivor;
/**
 * 
 */
UCLASS()
class MYPROP_API AKiller : public AMyCharacter
{
	GENERATED_BODY()

public:
		AKiller();

private:
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void BeginPlay() override;

	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);


public:
	//캐릭터 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FKillerInfo m_Info;

	float m_PrevSP;

	FKillerInfo* GetInfo() { return &m_Info; }
	void SetInfo(FKillerInfo new_Info) { m_Info = new_Info; }
	
	//Q 공격
	void RangeAttack();
	void Attack();
	void AttackAction();
};
