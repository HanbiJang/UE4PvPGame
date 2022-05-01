// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
//분할구현
#include "Survivor_Move.h"
#include "Survivor_Change.h"
#include "Multi/Survivor_Multi.h"

ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	fRunPower(5),
	bChangeEnable(false),
	fJumpPower(500),
	FRChange(0, 0, 0),
	fRoPower(0.1f),
	isGround(false),
	JumpCnt(0)
{

	//변신용 매시 생성==================================================================
	m_PlayerObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_PlayerObject->SetupAttachment(RootComponent);
		//설정
	m_PlayerObject->SetLinearDamping(0.15);
	m_PlayerObject->SetAngularDamping(3.0);
	m_PlayerObject->SetNotifyRigidBodyCollision(true);

	//사운드 가져오기===================================================================
	ConstructorHelpers::FObjectFinder<USoundWave> HeartBeatSoundAsset(TEXT("SoundWave'/Game/Music/HeartBeat_Fast.HeartBeat_Fast'"));

	if (HeartBeatSoundAsset.Succeeded())
		SW_HeartBeat = HeartBeatSoundAsset.Object;

	ConstructorHelpers::FObjectFinder<USoundWave> ChaseSoundAsset(TEXT("SoundWave'/Game/Music/HorrorChaseMusic.HorrorChaseMusic'"));

	if (ChaseSoundAsset.Succeeded())
		SW_Chase = ChaseSoundAsset.Object;

	//오디오 컴포넌트 부착
	AC_HeartBeat = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_HeartBeat"));
	AC_Chase = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Chase"));
	AC_HeartBeat->AttachTo(GetMesh());
	AC_Chase->AttachTo(GetMesh());

	//멀티플레이 리플리케이션 설정
	bReplicates = true;
	GetMesh()->SetIsReplicated(true); //스켈레탈 매시
	m_PlayerObject->SetIsReplicated(true); //사물 매시

}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);

	//인간폼 변신
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToPlayer);
}

void ASurvivor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	////서버의 물리 기준으로 함
	//if (HasAuthority()) {

	//}

	if (m_state == EPLAYER_STATE::OBJECT && !isGround) {
		//내적을 이용해 두 충돌 사이의 각을 구하기
		float sizes = abs(HitNormal.Size() * FVector(0, 0, 1).Size()); //벡터의 크기
		float dot = FVector::DotProduct(FVector(0, 0, 1), HitNormal); //내적값
		float angle = FMath::RadiansToDegrees(FMath::Acos(dot / sizes));
		if (angle <= 45) {
			isGround = true;
			JumpCnt = 0;
		}
	}
}

void ASurvivor::Interaction()
{
	//발전기, 문 등과 상호작용
}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();

	//======플레이어 (초기) 데이터 설정==================================================
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		SetInfo(*(GI->GetSurvivorInfo(TEXT("Survivor1"))));
	}

	//킬러 가져오기 =====================================================================
	//월드 상의 특정 클래스 Actor을 가져오기
	TArray<AActor*> arrActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKiller::StaticClass(), arrActor);
	for (int i = 0; i < arrActor.Num(); i++) {
		arrKiller.Add(Cast<AKiller>(arrActor[i])); //형변환
	}

	//사운드 스폰하기
	//AC_HeartBeat = UGameplayStatics::SpawnSound2D(this, SW_HeartBeat);
	AC_HeartBeat->SetSound(SW_HeartBeat);
	//AC_Chase = UGameplayStatics::SpawnSound2D(this, SW_Chase);
	AC_Chase->SetSound(SW_Chase);
	AC_HeartBeat->Stop();
	AC_Chase->Stop();
}

void ASurvivor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//체력, 스테미나 실시간 업데이트
	if (m_PrevHP != m_Info.fCurHP || m_PrevSP != m_Info.fCurSP)
	{
		// 이전 체력과 현제 체력이 다르다.
		// HUD 갱신
		AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		float HPRatio = m_Info.fCurHP / m_Info.fMaxHP;
		float MPRatio = m_Info.fCurSP / m_Info.fMaxSP;
		if (GM != nullptr)
		GM->UpdatePlayHUD(HPRatio, MPRatio);
	}

	// 현재 체력, 스태미너를 이전프레임 체력으로 갱신
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;

	//대시 가능 여부)
	if (GetInfo()->fCurSP < 0.1) {
		isDashEnable = false;
	}

	//대시 중)
	//[1] 스태미너 감소
	if (isDashEnable && isDashPressed && isMoving) {
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //속도 증가 (대시)	
		ChangeState(EPLAYER_STATE::DASH); //상태 전환
		isDashed = true;

		GetInfo()->fCurSP -= 0.4f;
		if (GetInfo()->fCurSP < 0) GetInfo()->fCurSP = 0;
	}
	//[2] 대시 아닌 상태) 스태미너 회복
	else{ 
		GetCharacterMovement()->MaxWalkSpeed = 600.f; //속도 원상복귀
		isDashed = false;

		GetInfo()->fCurSP += 0.035f;
		if (GetInfo()->fCurSP > GetInfo()->fMaxSP) GetInfo()->fCurSP = GetInfo()->fMaxSP;
	}

	//======================================================================================

	//살인마와 거리 차이에 따라서 비네팅 효과 & 화면 그레인 (지터) 증가 감소
	for (int i = 0; i < arrKiller.Num(); i++) {

		if ((m_state != EPLAYER_STATE::OBJECT && this->GetDistanceTo(arrKiller[i]) < 500) 
			|| (m_state == EPLAYER_STATE::OBJECT && MyGetDistance(m_PlayerObject->GetRelativeLocation(), arrKiller[i]->GetActorLocation()) < 500)) {

			m_Cam->PostProcessSettings.VignetteIntensity += 0.05;
			if (m_Cam->PostProcessSettings.VignetteIntensity > 2) m_Cam->PostProcessSettings.VignetteIntensity = 2;

			m_Cam->PostProcessSettings.GrainIntensity += 0.05;
			if (m_Cam->PostProcessSettings.GrainIntensity > 1) m_Cam->PostProcessSettings.GrainIntensity = 1;

			m_Cam->PostProcessSettings.GrainJitter += 0.05;
			if (m_Cam->PostProcessSettings.GrainJitter > 1) m_Cam->PostProcessSettings.GrainJitter = 1;

			//사운드 조절
			if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this && !AC_HeartBeat->IsPlaying() && !AC_Chase->IsPlaying()) {
				//UE_LOG(LogTemp, Log, TEXT("Killer Name!!! : %s"), *(arrKiller[i]->GetName()));
				//UE_LOG(LogTemp, Log, TEXT("Killer distance!!! : %f"), GetDistanceTo(arrKiller[i]));

				AC_HeartBeat->Play();
				AC_Chase->Play();
			}
			
		}
		else {
			m_Cam->PostProcessSettings.VignetteIntensity -= 0.05;
			if (m_Cam->PostProcessSettings.VignetteIntensity <0 ) m_Cam->PostProcessSettings.VignetteIntensity = 0;

			m_Cam->PostProcessSettings.GrainIntensity -= 0.05;
			if (m_Cam->PostProcessSettings.GrainIntensity < 0) m_Cam->PostProcessSettings.GrainIntensity = 0;

			m_Cam->PostProcessSettings.GrainJitter -= 0.05;
			if (m_Cam->PostProcessSettings.GrainJitter < 0) m_Cam->PostProcessSettings.GrainJitter = 0;

			//사운드 조절
			if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this && AC_HeartBeat->IsPlaying() && AC_Chase->IsPlaying()) {
				AC_HeartBeat->Stop();
				AC_Chase->Stop();
			}
		}
	}

	//죽기=======================================================================
	if (GetInfo()->fCurHP <= 0) ChangeState(EPLAYER_STATE::DEAD);
}

void ASurvivor::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
}

void ASurvivor::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
}

void ASurvivor::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{
}





