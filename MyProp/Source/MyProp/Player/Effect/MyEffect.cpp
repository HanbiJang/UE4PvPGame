// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffect.h"

// Sets default values
AMyEffect::AMyEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	SetRootComponent(m_Particle);
}

// Called when the game starts or when spawned
void AMyEffect::BeginPlay()
{
	Super::BeginPlay();

	//이펙트 설정하기
	UParticleSystem* ParticleAsset = nullptr;
	switch (m_eType)
	{

	case EKillerEffect::ATTACK:
		ParticleAsset = LoadObject<UParticleSystem>(GetWorld(), TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
		break;
	case EKillerEffect::Q:
		ParticleAsset = LoadObject<UParticleSystem>(GetWorld(), TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_magicCircle1.P_ky_magicCircle1'"));
		break;
	case EKillerEffect::E:
		ParticleAsset = LoadObject<UParticleSystem>(GetWorld(), TEXT(""));
		break;
	case EKillerEffect::RIGHTCLICK:
		ParticleAsset = LoadObject<UParticleSystem>(GetWorld(), TEXT(""));
		break;
	case EKillerEffect::PARTICLEHIT:
		ParticleAsset = LoadObject<UParticleSystem>(GetWorld(), TEXT("ParticleSystem'/Game/Blueprints/Killer/Projectile/P_ky_ThunderBallHit_NoThunder.P_ky_ThunderBallHit_NoThunder'"));
		break;
	}

	if (nullptr != ParticleAsset)
	{
		m_Particle->SetTemplate(ParticleAsset);
		m_Particle->OnSystemFinished.AddDynamic(this, &AMyEffect::OnFinish);
	}
	else UE_LOG(LogTemp, Log, TEXT("null effect rangeAttack"));

}

// Called every frame
void AMyEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyEffect::OnFinish(UParticleSystemComponent* _Particle)
{
	Destroy();
}

