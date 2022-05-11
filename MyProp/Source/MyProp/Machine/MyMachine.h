// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyMachine.generated.h"


class AKiller;
class ASurvivor;

UCLASS()
class MYPROP_API AMyMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() //Box에 할당되는 이벤트
		void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);

	//매시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Mesh; //사물형 스테틱 매시 (물리 기능 있음)
	//컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_Cap;

	//포인트 라이트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* m_Light;

	UStaticMesh* m_DoneMesh;

	bool IsEnable; //수리가 가능한지
	float Range; //수리 거리
	float RepairTime; //총 수리해야하는 시간
	float CurRepairTime; //현재 수리해야하는 시간
	float IsDone; //수리 완료 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RepairBox;
	ASurvivor* sur; //1명 생존자


	UFUNCTION(Reliable, Client)
		void MachineUIVisiblity_Client(bool b, ASurvivor* _sur);
};
