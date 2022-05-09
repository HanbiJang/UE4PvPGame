// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyInfo.h" //캐릭터 정보 
#include <EngineMinimal.h>
#include "MyCharacterState.h"
#include "../MyGameInstance.h"

#include "Kismet/KismetMathLibrary.h"

//멀티플레이
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UMyAnimInstance; //중복 헤더 포함 문제 해결
class AMyPlayerObjectPawn; //중복 헤더 포함 문제 해결
class ASurvivor;

UCLASS()
class MYPROP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/// <summary>
	/// 1. 캐릭터 매시 설정,
	/// </summary>
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input	
	/// <summary>
	/// 입력키를 함수와 연결하기
	/// </summary>
	/// <param name="PlayerInputComponent"></param>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//대시
	virtual void Dash();
	virtual void DashStop();

	UPROPERTY(replicated)
		bool isMoving;

	UPROPERTY(replicated)
		bool isJumping;

	UPROPERTY(replicated)
		bool isDashed;
	UPROPERTY(replicated)
		bool isDashPressed;
	UPROPERTY(replicated)
		bool isDashEnable;

	//값 바꾸기(서버)
	UFUNCTION(Reliable, Server)
		void SetisDashEnable_Server(bool value);

	//값 바꾸기(멀티캐스트)
	UFUNCTION(Reliable, NetMulticast)
		void SetisDashEnable_Multicast(bool value);

	//값 바꾸기(서버)
	UFUNCTION(Reliable, Server)
		void SetisDashPressed_Server(bool value);

	//값 바꾸기(멀티캐스트)
	UFUNCTION(Reliable, NetMulticast)
		void SetisDashPressed_Multicast(bool value);

	//값 바꾸기(서버)
	UFUNCTION(Reliable, Server)
		void SetisDashed_Server(bool value);

	//값 바꾸기(멀티캐스트)
	UFUNCTION(Reliable, NetMulticast)
		void SetisDashed_Multicast(bool value);

	//값 바꾸기(서버)
	UFUNCTION(Reliable, Server)
		void SetisMoving_Server(bool value);

	//값 바꾸기(멀티캐스트)
	UFUNCTION(Reliable, NetMulticast)
		void SetisMoving_Multicast(bool value);

	UPROPERTY(replicated)
		bool isObject = false; //사물형태인지 아닌지

	//Machine Interaction
	void Interaction();

	//Jump
	//UFUNCTION(Reliable, Server)
	//virtual void myJump();

	//변신용 이동 ==========
	virtual void UpDown(float f);
	virtual void LeftRight(float f);
	virtual void MyJump();

	//카메라
	virtual void Turn(float f);
	virtual void LookUp(float f);

	//=======
	void JumpAction();

	//ItemBtn
	void Item1();
	void Item2();

private:
	float jumpCntTime = 0.f; //점프 할 때마다 시간을 잰다 
	const float jumpEnableTime = 2.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Cam;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;
private:
	UPROPERTY(replicated)
	float fLeftRight;
	UPROPERTY(replicated)
	float fUpdown;


protected:
	UMyAnimInstance* m_AnimInst; //클래스 객체 받아올 것


public:
	//캐릭터의 상태
		//리플리케이션 설정
	UPROPERTY(replicated, ReplicatedUsing = OnRep_State, EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_state;

public:
	//캐릭터 상태 전환
	UFUNCTION(Reliable, Server)
		virtual void ChangeState(EPLAYER_STATE newState);

	UFUNCTION(Reliable, NetMulticast)
		void ChangeState_Multicast(EPLAYER_STATE newState);

public:
	EPLAYER_STATE GetState() { return m_state; }

protected:
	//데이터 테이블
	UDataTable* m_PlayerTable;

public:
	//캐릭터 정보 옮기기
	virtual void ShareInfoToObject(AMyPlayerObjectPawn* m_PlayerObjectPawn, FTableRowBase data) {}; //인간 -> 사물
	virtual void SetInfoToCharacter(AMyPlayerObjectPawn* m_PlayerObjectPawn,FTableRowBase data) {}; //사물 -> 인간

	//멀티플레이==============================================
	UFUNCTION()
		void OnRep_State();

	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//뚝배기 스턴============================================
	virtual void TurnMove() {};
};
