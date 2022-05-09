// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <MyProp/Player/MyInfo.h>
#include <MyProp/Player/Killer/MyProjectile.h>

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Killer.generated.h"

class AMyPlayerController;
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
	void Tick(float DeltaTime) override;
	void BeginPlay() override;

	UFUNCTION() //HeadBox에 할당되는 이벤트
		void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);


public:
	//캐릭터 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FKillerInfo m_Info;

	//float m_PrevSP;

	FKillerInfo* GetInfo() { return &m_Info; }
	void SetInfo(FKillerInfo new_Info) { m_Info = new_Info; }

public:
	//무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Weapon;
	UStaticMesh* BasicWeaponMesh;

	void RangeAttack();
	void RangeAttackAction(); //실제 구현부
	void Attack();
	void AttackAction();
	void RCAttack();
	void RCAttackAction(); //실제 구현부
	void EAttack();
	void EAttackAction(); //실제 구현부

private:
	//공격 쿨타임 관련
	AMyPlayerController* PC;

	//[1] 평타
	bool bAttackEnable;
	FTimerHandle FAttackTimer; //공격 타이머
	void SetAttackEnable(){ bAttackEnable = true; }
	float attackSpeed;

	//[2] Q공격
	bool bRangeAttackEnable;
	FTimerHandle FRangeAttackTimer; //공격 타이머
	void SetRangeAttackEnable() { bRangeAttackEnable = true; }
	//float fCurQLeftTime; //남은 시간
	float fPrevQLeftTime; //남은 시간 (이전)

	//[3] 우클릭 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyProjectile> m_RCAttackProjectile; //우클공격 파티클
	bool bRCAttackEnable;
	FTimerHandle FRCAttackTimer; //공격 타이머
	void SetRCAttackEnable() { bRCAttackEnable = true; }
	//float fCurRCLeftTime; //남은 시간
	float fPrevRCLeftTime; //남은 시간 (이전)

	//[4] E클릭 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyProjectile> m_EAttackProjectile; //우클공격 파티클
	bool bEAttackEnable;
	FTimerHandle FEAttackTimer; //공격 타이머
	void SetEAttackEnable() { bEAttackEnable = true; }
	//float fCurELeftTime; //남은 시간
	float fPrevELeftTime; //남은 시간 (이전)

	//attack effect
	UParticleSystem* m_AttackEffect;

private:
	UFUNCTION(Reliable, Server)
		void RangeAttackEffect_Server();
	UFUNCTION(Reliable, NetMulticast)
		void RangeAttackEffect_Multicast();

	//UI 업데이트 ============================
	UFUNCTION(Reliable, Server)
		void UpdateUI_Server();

	//(뚝) 피격시====================================================
	void TurnMove() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* HeadBox;
};

