// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Mode/MyPropGameModeBase.h>
#include "../Killer/Killer.h"

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

//분할구현 / 중복 헤더 포함 문제 해결
class Survivor_Move; 
class Survivor_Change;
class Survivor_Multi;

/**
 * 
 */
UCLASS()
class MYPROP_API ASurvivor : public AMyCharacter
{
	GENERATED_BODY()

public:
	ASurvivor();

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);

	//이동 (대시)
	void Dash() override;
	void DashStop() override;

	//상호작용
	void Interaction();

public:

	//캐릭터 정보
	//리플리케이션 설정
	UPROPERTY(ReplicatedUsing = OnRep_Info, EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;

	float m_PrevHP;
	float m_PrevSP;

	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }
	
	//킬러 정보
	TArray<AKiller*> arrKiller;

//변신 기능
//캐릭터의 오브젝트 전환
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_PlayerObject; //사물형 스테틱 매시 (물리 기능 있음)

	UPROPERTY(replicated)
	FTimerHandle FPhysicsTimer; //물리 작용 끔/켬 타이머
	UPROPERTY(replicated)
	FTimerHandle FChangeEnableTimer; //변신 가능 시간 끔/켬 타이머
	void SetSimulatePhysicsTrue() { m_PlayerObject->SetSimulatePhysics(true); }

	//변신 가능 상태 나타내는 변수 (변신 과다 사용 조절용)
	UPROPERTY(replicated)
	bool bChangeEnable;
	void SetbChangeEnableTrue() { bChangeEnable = true; }

	//변신 초기화 각도, 초기화 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;
	void ChangeToObject(UStaticMesh* mesh, FVector fscale); //사람->오브젝트 변신
	void ChangeObjectMesh(UStaticMesh* mesh, FVector scale); //오브젝트 -> 새오브젝트 변신
	void ChangeToPlayer(); //오브젝트->사람 변신

	//변신용 이동
	virtual void UpDown(float f) override;
	virtual void LeftRight(float f) override;
	virtual void Jump() override;
	//이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRunPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRoPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fJumpPower;
	
	//Jump 관련 변수
	UPROPERTY(replicated)
	bool isGround;

	//2단 점프 제한
	UPROPERTY(replicated)
	int JumpCnt = 0;

	//땅과 닿았을 시 isGround를 true로 만든다
protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	//거리구하기 함수
	float MyGetDistance(FVector a, FVector b) {
		float res = sqrtf(powf(a.X - b.X, 2) + powf(a.Y - b.Y, 2) + powf(a.Z - b.Z, 2));
		return res;
	}

public:
	//사운드
	//추격 심장박동 사운드 에셋
	USoundWave* SW_HeartBeat;
	//추격 BGM 사운드 에셋
	USoundWave* SW_Chase;
	//사운드 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AC_HeartBeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AC_Chase;

	//멀티플레이==============================
	//캐릭터 Info를 공유하기
	UFUNCTION()
		void OnRep_Info();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Response to health being updated. 
	Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnInfoUpdate();

	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
