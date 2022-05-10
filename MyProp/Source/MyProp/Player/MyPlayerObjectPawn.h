// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Player/Common/MyInfo.h>
#include <EngineMinimal.h>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayerObjectPawn.generated.h"

//class AMyCharacter; //중복헤더 문제 해결
class ASurvivor;

UCLASS()
class MYPROP_API AMyPlayerObjectPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayerObjectPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//매시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_ObjectMesh;

	//카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	//이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRunPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRoPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fJumpPower;

	//Moving
	void UpDown(float f);
	void LeftRight(float f);

	//Machine Interaction
	void Interaction();

	//변신
	void PlayerObject();
	ASurvivor* pCharacter; //플레이어 인간폼 정보
	void SetPCharacter(ASurvivor* new_pCharacter){ pCharacter = new_pCharacter;  };

	//Jump
	void Jump();

	//Jump 관련 변수
	bool isGround;
	//2단 점프 제한
	int JumpCnt = 0;

	//땅과 닿았을 시 isGround를 true로 만든다
protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	//변신 초기화 각도, 초기화 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;

	//재 변신
	void ChangeObjectMesh(UStaticMesh* mesh, FVector scale);

	FTimerHandle FPhysicsTimer; //물리 작용 끔/켬 타이머
	void SetSimulatePhysicsTrue() { m_ObjectMesh->SetSimulatePhysics(true); }

	//변신 가능 상태 나타내는 변수 (변신 과다 사용 조절용)
	bool bChangeEnable;
	FTimerHandle FChangeEnableTimer; //변신 가능 시간 끔/켬 타이머
	void SetbChangeEnableTrue() { bChangeEnable = true; }


private:
	//캐릭터 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;
public:
	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }
};
