// Fill out your copyright notice in the Description page of Project Settings.


#include "KillerRCProjectile.h"
#include "Components/StaticMeshComponent.h"
#include <MyProp/Player/MyEffectManager.h>
#include <MyProp/Player/Survivor/Survivor.h> 


AKillerRCProjectile::AKillerRCProjectile()
	: m_fAccTime(0.f)
	, m_Sound(nullptr)
	, m_Attenuation(nullptr) 
{
	isHit = false;

	//매시 설정
	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_MeshComponent->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(
		TEXT("StaticMesh'/Game/PolygonPrototype/Meshes/Props/SM_Prop_Knife_01.SM_Prop_Knife_01'"));
	if (meshAsset.Succeeded()) {	
		m_Mesh = meshAsset.Object;
		m_MeshComponent->SetStaticMesh(m_Mesh);
	}

	//파티클 에셋 설정
	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAsset(
		TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_waterBall.P_ky_waterBall'"));
	if (particleAsset.Succeeded())
	{
		GetParticle()->SetTemplate(particleAsset.Object);
	}

	m_Particle->SetupAttachment(m_MeshComponent);

	//newScale = GetActorScale3D();

	//// 재생할 사운드 참조
	//ConstructorHelpers::FObjectFinder<USoundBase> soundasset(TEXT("SoundWave'/Game/Interface_And_Item_Sounds/WAV/Bar_Filling_02.Bar_Filling_02'"));
	//if (soundasset.Succeeded())
	//{
	//	m_Sound = soundasset.Object;
	//}

	//ConstructorHelpers::FObjectFinder<USoundAttenuation> soundAtt(TEXT("SoundAttenuation'/Game/BlueprintClass/DefaulAttenuation.DefaulAttenuation'"));
	//if (soundAtt.Succeeded())
	//{
	//	m_Attenuation = soundAtt.Object;
	//}
}


// Called when the game starts or when spawned
void AKillerRCProjectile::BeginPlay() {
	Super::BeginPlay();

	//기능 연결
	GetBox()->OnComponentHit.AddDynamic(this, &AKillerRCProjectile::OnHit);
	GetBox()->OnComponentBeginOverlap.AddDynamic(this, &AKillerRCProjectile::OnBeginOverlap);
}


// Called every frame
void AKillerRCProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_fAccTime += DeltaTime;
	if (3.f <= m_fAccTime)
	{
		Destroy();
	}
}

void AKillerRCProjectile::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp
	, FVector _NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(_HitComponent, _OtherActor, _OtherComp, _NormalImpulse, Hit);
	
	//// 소리 재생, 2D
	//UGameplayStatics::PlaySoundAtLocation(
		//GetWorld(), 
		//m_Sound, 
		//GetActorLocation(), 1.f, 1.f, 0.f, m_Attenuation);

	//floor && 캐릭터 자신 충돌 없애기 (에디터 콜리전 설정)
	isHit = true;

	UE_LOG(LogTemp, Log, TEXT("%s"), *(_OtherActor->GetName()));

	//이펙트 생성 & 재생
	FTransform trans(GetActorRotation(), GetActorLocation() );
	UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::PARTICLEHIT, trans, GetLevel());
	
	//캐릭터에게 피해입히기
	ASurvivor* survivor = Cast<ASurvivor>(_OtherActor);
	if (survivor != nullptr) { //cast 실패시 null
		//생존자가 맞게하기
		UGameplayStatics::ApplyDamage(survivor, 15.f, NULL, GetOwner(), NULL);
	}

	Destroy();

}

void AKillerRCProjectile::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{

}

void AKillerRCProjectile::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{

}