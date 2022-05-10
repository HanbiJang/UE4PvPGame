// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MyProp/Player/Killer/Projectile/MyProjectile.h>
#include "KillerRCProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API AKillerRCProjectile : public AMyProjectile
{
	GENERATED_BODY()

		float m_fAccTime;
	
public:
	AKillerRCProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundBase* m_Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundAttenuation* m_Attenuation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_MeshComponent;

public:
	UStaticMeshComponent* GetMeshComponent() { return m_MeshComponent; };

	//에셋
	UStaticMesh* m_Mesh;


public:
	virtual void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit) override;

	// Collision Event
public:
	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) override;
	void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex) override;

	
};
