// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <MyProp/Player/MyInfo.h>
#include "Particles/ParticleSystemComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffect.generated.h"


UCLASS()
class MYPROP_API AMyEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* m_Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		EKillerEffect			  m_eType;

public:
	void SetEffectType(EKillerEffect _eType) { m_eType = _eType; }

public:
	UFUNCTION()
		void OnFinish(UParticleSystemComponent* _Particle);

};
