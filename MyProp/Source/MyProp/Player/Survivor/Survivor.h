// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Mode/MyPropGameModeBase.h>
#include "../Killer/Killer.h"
#include "Materials/MaterialInterface.h"

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

//분할구현 / 중복 헤더 포함 문제 해결
class Survivor_Move; 
class Survivor_Change;
class Survivor_Multi;
class AMyPlayerObject;

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
	UPROPERTY(/*ReplicatedUsing = OnRep_Info,*/replicated,EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;

	UPROPERTY(replicated)
	float m_PrevHP;
	UPROPERTY(replicated)
	float m_PrevSP;

	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }

	AMyPlayerController* PC; //플레이어 컨트롤러
	
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

	//3인칭 변신 (사물 선택 레이캐스트)
	void SelectObject();
	//사물 외곽선 그리기 기능
	UFUNCTION(Reliable, Client)
		void DrawOutLine();

	bool isChangableObject = false;
	bool isChangableObjectSelected = false;
	AMyPlayerObject* pPlayerObject; //new
	AMyPlayerObject* pPlayerObject_old; //old

	//변신 초기화 각도, 초기화 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;

	UFUNCTION(Reliable, Server)
	void ChangeToObject(/*UStaticMesh* myMesh, FVector fscale*/); //사람->오브젝트 변신
	UFUNCTION(Reliable, NetMulticast)
	void ChangeToObject_Multicast(/*UStaticMesh* myMesh, FVector fscale*/); //사람->오브젝트 변신

	UFUNCTION(Reliable, Server)
	void ChangeObjectMesh(/*UStaticMesh* myMesh, FVector scale*/); //오브젝트 -> 새오브젝트 변신
	UFUNCTION(Reliable, NetMulticast)
	void ChangeObjectMesh_Multicast(/*UStaticMesh* myMesh, FVector scale*/); //오브젝트 -> 새오브젝트 변신
	
	UFUNCTION(Reliable, Server)
	void ChangeToPlayer(); //오브젝트->사람 변신
	UFUNCTION(Reliable, NetMulticast)
	void ChangeToPlayer_Multicast(); //오브젝트 -> 새오브젝트 변신

	//변신용 이동
	virtual void UpDown(float f) override;
	virtual void LeftRight(float f) override;
	virtual void MyJump() override;

	//오브젝트용 이동 (서버에서만 동작)
	UFUNCTION(Reliable, Server)
		virtual void UpDown_Server(float f);
	UFUNCTION(Reliable, Server)
		virtual void LeftRight_Server(float f);
	UFUNCTION(Reliable, Server)
		virtual void MyJump_Server();

	//오브젝트용 이동 (서버, 클라이언트 동시 동작)
	UFUNCTION(Reliable, NetMulticast)
	virtual void UpDown_Multicast(float f);
	UFUNCTION(Reliable, NetMulticast)
	virtual void LeftRight_Multicast(float f);
	UFUNCTION(Reliable, NetMulticast)
	virtual void MyJump_Multicast();

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

	//변신
	UFUNCTION(Reliable, Server)
		void SetBodyLocation();

	UFUNCTION(Reliable, NetMulticast)
		void SetBodyLocation_Multicast();

	//스태미나 업데이트
	UFUNCTION(Reliable, Server)
		void UpdateSP();

	//UFUNCTION(Reliable, Server)
	//	void UseSP();

	//비네팅 공포 효과
	UFUNCTION(Reliable, Server)
		void ShowVinetting();

	UFUNCTION(Reliable, Client)
		void UpdateSP_Client();

	/*UFUNCTION(Reliable, Client)*/
		void UseSP/*_Client*/();

	UFUNCTION(Reliable, Server)
		void DashSpeedUp_Server();
	UFUNCTION(Reliable, NetMulticast)
		void DashSpeedUp_Multicast();

	UFUNCTION(Reliable, Server)
		void DashSpeedDown_Server();
	UFUNCTION(Reliable, NetMulticast)
		void DashSpeedDown_Multicast();

	//비네팅 효과
	UFUNCTION(Reliable, Client)
		void ShowVinetting_Client();

	//맞기 효과
	//몸이 빨개지기!
	void TurnRed();
	void TurnOriginalColor();

	UFUNCTION(Reliable, Server)
		void HitColorReaction_Server(); //빨개졌다 다시 돌아옴
	UFUNCTION(Reliable, NetMulticast)
		void HitColorReaction_Multicast(); //빨개졌다 다시 돌아옴


	UMaterialInterface* mat_hit_object;
	UMaterialInterface* mat_hit_human;
	UMaterialInterface* mat_original;
	UPROPERTY(replicated)
		FTimerHandle FHitRedTimer; //몇초 뒤에 히트 매시에서 오리지널 매시로 돌아오는 타이머

	void TurnMove() override;
public:
	//발전기 수리
	bool IsRepairEnable;
	void SetIsRepairEnable(bool value) { IsRepairEnable = value; };

	////킬러 크기
	//UFUNCTION(Reliable, Server)
	//	void SetKillerScale_Server(AKiller* killer,float f);
	//UFUNCTION(Reliable, NetMulticast)
	//	void SetKillerScale_Multicast(AKiller* killer,float f);
	
};
