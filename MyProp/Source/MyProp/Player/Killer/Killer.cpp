// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"
#include <DrawDebugHelpers.h>
#include "../Survivor/Survivor.h" 
#include <MyProp/Player/Effect/MyEffectManager.h>
#include <MyProp/Player/Killer/Projectile/KillerRCProjectile.h>
#include <MyProp/Controller/MyPlayerController.h>

AKiller::AKiller():
	bAttackEnable(true),
	attackSpeed(2.f), //평타 쿨타임
	bRangeAttackEnable(true),
	bRCAttackEnable(true),
	bEAttackEnable(true)

{
	//멀티플레이 - 리플리케이션 설정
	bReplicates = true;
	GetMesh()->SetIsReplicated(true); //스켈레탈 매시

	//칼들고 있게 하기
	m_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	m_Weapon->SetupAttachment(GetMesh(), TEXT("hand_r_weapon"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> BasicWeapon(TEXT("StaticMesh'/Game/PolygonPrototype/Meshes/Props/SM_Prop_Knife_01.SM_Prop_Knife_01'"));

	if (BasicWeapon.Succeeded()) {
		BasicWeaponMesh = BasicWeapon.Object;
		m_Weapon->SetStaticMesh(BasicWeaponMesh);
	}	

	//투사체 블루프린트 설정 
	ConstructorHelpers::FClassFinder<AMyProjectile> BPProjectile(
		TEXT("Blueprint'/Game/Blueprints/Killer/Projectile/BP_KillerRCAttackProjectile.BP_KillerRCAttackProjectile_C'"));

	if (BPProjectile.Succeeded()) {
		m_RCAttackProjectile = BPProjectile.Class;
	}

	//뚝배기
	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(GetMesh(), TEXT("head"));

}

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 왼클릭 - 공격 키
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);
	// Q키 - 범위 공격
	PlayerInputComponent->BindAction(TEXT("Q"), EInputEvent::IE_Pressed, this, &AKiller::RangeAttack);
	// E키 -  공격
	PlayerInputComponent->BindAction(TEXT("E"), EInputEvent::IE_Pressed, this, &AKiller::EAttack);
	// 우클릭 - 투척 공격
	PlayerInputComponent->BindAction(TEXT("RCAttack"), EInputEvent::IE_Pressed, this, &AKiller::RCAttack);
}

void AKiller::BeginPlay() {
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//======플레이어 (초기) 데이터 설정======
	if (GI != nullptr) {
		SetInfo(*(GI->GetKillerInfo(TEXT("Killer1"))));
	}

	PC = Cast<AMyPlayerController>(this->GetInstigatorController());

	//======뚝스턴======
	//GetBox()->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	HeadBox->OnComponentBeginOverlap.AddDynamic(this, &AKiller::OnBeginOverlap);
}

void AKiller::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(HasAuthority())
	UpdateUI_Server();

	//쿨타임 시간 계산
	if (!bRangeAttackEnable) {
		//Clamp: 0과 maxHP 값 사이에서 설정함
		m_Info.fCurQLeftTime = FMath::Clamp(m_Info.fCurQLeftTime-DeltaTime, 0.f, m_Info.rangeAttackSpeed);		
	}

	if (!bEAttackEnable) {
		m_Info.fCurELeftTime = FMath::Clamp(m_Info.fCurELeftTime - DeltaTime, 0.f, m_Info.EAttackSpeed);
	}

	if (!bRCAttackEnable){
		m_Info.fCurRCLeftTime = FMath::Clamp(m_Info.fCurRCLeftTime - DeltaTime, 0.f, m_Info.rCAttackSpeed);
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
	//공격 모션
	if (bRangeAttackEnable) {
		ChangeState(EPLAYER_STATE::RANGEATTACK);
		bRangeAttackEnable = false;

		RangeAttackEffect_Server(); //시전 이펙트

		//쿨타임 UI 초기화
		m_Info.fCurQLeftTime = m_Info.rangeAttackSpeed;
		//rangeAttackSpeed초 뒤에 Timer 켜기
		GetWorld()->GetTimerManager().SetTimer(FRangeAttackTimer, this, &AKiller::SetRangeAttackEnable, m_Info.rangeAttackSpeed, false);
	}	
}

void AKiller::RangeAttackAction() {

	//공격 (충돌) Trace 채널을 이용한 범위 체크	
	float fRadius = 300.f; //구의 크기
	FVector attackPos = GetActorLocation();
	ASurvivor* survivor;

	TArray<FHitResult> arrHit; //충돌 결과
	FCollisionQueryParams param(NAME_None, false, this);
	//레이
	GetWorld()->SweepMultiByChannel(arrHit, attackPos, attackPos, FQuat::Identity
		, ECC_GameTraceChannel3/*Attack Trace Channel*/
		, FCollisionShape::MakeSphere(fRadius), param);

	if (arrHit.Num())
	{
		for (int i = 0; i < arrHit.Num(); i++) {
			survivor = Cast<ASurvivor>(arrHit[i].GetActor());

			//생존자든 아니든, 맞은 부위에 이펙트 표시하기
			FTransform trans(GetActorRotation(), arrHit[i].ImpactPoint);
			UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::ATTACK, trans, GetLevel());

			UE_LOG(LogTemp, Log, TEXT("%s"), *(arrHit[i].GetActor()->GetName()) );

			if (survivor != nullptr) { //cast 실패시 null
				//생존자가 맞게하기
				UGameplayStatics::ApplyDamage(survivor, 5.f, NULL, GetOwner(), NULL);
				break;
			}
		}
	}

#ifdef ENABLE_DRAW_DEBUG //범위를 눈으로 확인
	FColor color;
	arrHit.Num() ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), attackPos, fRadius, 12, color, false, 2.5f);
#endif

}

void AKiller::RCAttack() {
	//공격 모션
	if (bRCAttackEnable) {
		ChangeState(EPLAYER_STATE::RCATTACK);
		bRCAttackEnable = false;

		//쿨타임 UI 초기화
		m_Info.fCurRCLeftTime = m_Info.rCAttackSpeed;
		//attackSpeed초 뒤에 Timer 켜기
		GetWorld()->GetTimerManager().SetTimer(FRCAttackTimer, this, &AKiller::SetRCAttackEnable, m_Info.rCAttackSpeed, false);
	}
}
void AKiller::RCAttackAction() {
	//실제 구현부
	//투사체 소환, 투사체가 날아가서 생존자 맞기 판정

	// 투사체 생성
	FActorSpawnParameters SpawnParam = {};
	SpawnParam.OverrideLevel = GetLevel();
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	//블루프린트로 투사체 설정
	if (m_RCAttackProjectile != nullptr) {
		AKillerRCProjectile* pRCAttackProjectile = GetWorld()->SpawnActor<AKillerRCProjectile>(m_RCAttackProjectile
			, GetActorLocation() + GetActorForwardVector() * 50.f
			, GetActorForwardVector().Rotation()
			, SpawnParam);
		if (pRCAttackProjectile != nullptr) {
			// begin play 호출
			pRCAttackProjectile->FinishSpawning(pRCAttackProjectile->GetTransform());
		}
	}

}

void AKiller::RangeAttackEffect_Server_Implementation() {
	RangeAttackEffect_Multicast();
}

void AKiller::RangeAttackEffect_Multicast_Implementation() {
	
	//이펙트 재생	
	FTransform trans(GetActorRotation(), GetActorLocation() - FVector(0, 0, (GetMesh()->Bounds.GetBox().GetSize().Z) / 2));
	UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::Q, trans, GetLevel());
}

void AKiller::EAttack() {
	//공격 모션
	if (bEAttackEnable) {
		ChangeState(EPLAYER_STATE::EATTACK);
		bEAttackEnable = false;

		//쿨타임 UI 초기화
		m_Info.fCurELeftTime = m_Info.EAttackSpeed;
		//attackSpeed초 뒤에 Timer 켜기
		GetWorld()->GetTimerManager().SetTimer(FEAttackTimer, this, &AKiller::SetEAttackEnable, m_Info.EAttackSpeed, false);
	}
}
void AKiller::EAttackAction() {

}

void AKiller::UpdateUI_Server_Implementation() {
	//체력, 스테미나 실시간 업데이트
	if (fPrevRCLeftTime != m_Info.fCurRCLeftTime || fPrevELeftTime != m_Info.fCurELeftTime
		|| fPrevQLeftTime != m_Info.fCurQLeftTime)
	{
		// 이전 쿨타임이 현재 쿨타임과 다르면
		// HUD 갱신
		//AMyPlayerController* PC = Cast<AMyPlayerController>(this->GetInstigatorController());

		float CurQSkillRatio = m_Info.fCurQLeftTime / m_Info.rangeAttackSpeed;
		float CurESkillRatio = m_Info.fCurELeftTime / m_Info.EAttackSpeed;
		float CurRCSkillRatio = m_Info.fCurRCLeftTime / m_Info.rCAttackSpeed;

		if (PC != nullptr) PC->UpdatePlayHUD_Killer(CurQSkillRatio, CurESkillRatio, CurRCSkillRatio,
			m_Info.fCurQLeftTime, m_Info.fCurELeftTime, m_Info.fCurRCLeftTime);
	}

	// 현재 체력, 스태미너를 이전프레임 체력으로 갱신
	fPrevQLeftTime = m_Info.fCurQLeftTime;
	fPrevELeftTime = m_Info.fCurELeftTime;
	fPrevRCLeftTime = m_Info.fCurRCLeftTime;

}

void AKiller::TurnMove() {
	if ((!isDashed && !isJumping)) ChangeState(EPLAYER_STATE::MOVE);
}

//====================================================================
void AKiller::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) {
	UE_LOG(LogTemp, Log, TEXT(" AKiller OnBeginOverlap OnBeginOverlap OnBeginOverlap "));

	//박스 오버랩 이벤트 -> change Hit
	ASurvivor* sur = Cast<ASurvivor>(_OtherActor);
	if (sur)
		ChangeState(EPLAYER_STATE::HIT);

}
void AKiller::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex) {

}
void AKiller::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit) {
}
