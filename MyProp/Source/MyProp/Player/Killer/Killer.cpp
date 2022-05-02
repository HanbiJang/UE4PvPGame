// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"
#include <DrawDebugHelpers.h>
#include "../Survivor/Survivor.h" 
#include <MyProp/Player/MyEffectManager.h>

AKiller::AKiller():
	bAttackEnable(true),
	attackSpeed(2.f)
{
	//멀티플레이 - 리플리케이션 설정
	bReplicates = true;
	GetMesh()->SetIsReplicated(true); //스켈레탈 매시

	//평타 이펙트
	ConstructorHelpers::FObjectFinder<UParticleSystem> AttackEffectAsset(
		TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
	if (AttackEffectAsset.Succeeded())
	{
		//m_AttackEffect->SetTemplate(AttackEffectAsset.Object);
		//m_AttackEffect->OnSystemFinished.AddDynamic(this, &AKiller::OnFinish); //재생 후 삭제
	}
}

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 왼클릭 - 공격 키
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);
	// Q키 - 범위 공격
	PlayerInputComponent->BindAction(TEXT("Q"), EInputEvent::IE_Pressed, this, &AKiller::RangeAttack);
}

void AKiller::BeginPlay() {
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//======플레이어 (초기) 데이터 설정======
	if (GI != nullptr) {
		SetInfo(*(GI->GetKillerInfo(TEXT("Killer1"))));
	}
}

void AKiller::Attack()
{
	//공격 모션
	if (bAttackEnable) {
		ChangeState(EPLAYER_STATE::ATTACK);
		bAttackEnable = false;

		//attackSpeed초 뒤에 Timer 켜기
		GetWorld()->GetTimerManager().SetTimer(FAttackTimer, this, &AKiller::SetAttackEnable, attackSpeed, false);
	}	
}

void AKiller::AttackAction() 
{
	//공격 (충돌) Trace 채널을 이용한 범위 체크	
	float fRadius = 80.f; //구의 크기
	FVector attackPos = GetActorLocation() + FVector(0, 0, 50.f) + GetActorForwardVector()*100;
	ASurvivor* survivor;

	TArray<FHitResult> arrHit; //충돌 결과
	FCollisionQueryParams param(NAME_None, false, this);
	//레이
	GetWorld()->SweepMultiByChannel(arrHit, attackPos, attackPos, FQuat::Identity
		, ECC_GameTraceChannel3/*Attack Trace Channel*/
		, FCollisionShape::MakeSphere(fRadius), param);

	//bool bSurvivor = false;
	if (arrHit.Num())
	{
		for (int i = 0; i < arrHit.Num(); i++) {
			survivor = Cast<ASurvivor>(arrHit[i].GetActor());
			
			//생존자든 아니든, 맞은 부위에 이펙트 표시하기
			FTransform trans(GetActorRotation(), arrHit[i].Location);
			UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::ATTACK, trans, GetLevel());

			if (survivor != nullptr) { //cast 실패시 null
				//생존자가 맞게하기
				UGameplayStatics::ApplyDamage(survivor,30.f,NULL,GetOwner(),NULL);

				break;
			}
		}
	}

#ifdef ENABLE_DRAW_DEBUG //범위를 눈으로 확인
	//FColor color;
	//arrHit.Num() ? color = FColor::Red : color = FColor::Green;
	//DrawDebugSphere(GetWorld(), attackPos, fRadius, 12, color, false, 2.5f);
#endif

}

void AKiller::RangeAttack() {
	ChangeState(EPLAYER_STATE::RANGEATTACK);
	RangeAttackEffect_Server();
}

void AKiller::RangeAttackEffect_Server_Implementation() {
	RangeAttackEffect_Multicast();
}

void AKiller::RangeAttackEffect_Multicast_Implementation() {
	
	//이펙트 재생	
	FTransform trans(GetActorRotation(), GetActorLocation() - FVector(0, 0, (GetMesh()->Bounds.GetBox().GetSize().Z) / 2));
	UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::Q, trans, GetLevel());
}


void AKiller::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) {

}
void AKiller::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex) {

}
void AKiller::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit) {

}
