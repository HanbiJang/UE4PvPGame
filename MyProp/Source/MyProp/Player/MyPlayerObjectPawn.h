// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <EngineMinimal.h>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayerObjectPawn.generated.h"

class AMyCharacter; //중복헤더 문제 해결

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

	//move

	//jump 2회 가능


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
	AMyCharacter* pCharacter; //플레이어 인간폼 정보
	void SetPCharacter(AMyCharacter* new_pCharacter){ pCharacter = new_pCharacter;  };

	//Jump
	void Jump();

	//Jump 관련 변수
	bool isGround;

};
