// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectile.h"

// Sets default values
AMyProjectile::AMyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트 생성
	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	// 박스 컴포넌트를 루트씬 컴포넌트로 지정
	SetRootComponent(GetBox());
	// 파티클 컴포넌트가 박스를 부모로 지정
	GetParticle()->SetupAttachment(GetBox());

	// 투사체 업데이트 대상을 박스로 지정
	GetProjectile()->SetUpdatedComponent(GetBox());
}

// Called when the game starts or when spawned
void AMyProjectile::BeginPlay()
{
	Super::BeginPlay();

	////기능 연결
	//GetBox()->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	//GetBox()->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectile::OnBeginOverlap);
}

// Called every frame
void AMyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyProjectile::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{

}


