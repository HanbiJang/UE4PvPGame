// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
//분할구현
#include "Survivor_Move.h"
#include "Survivor_Change.h"
#include "Multi/Survivor_Multi.h"
#include <MyProp/Controller/MyPlayerController.h>
#include <MyProp/Object/MyPlayerObject.h>


ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	fRunPower(5),
	bChangeEnable(true),
	fJumpPower(500),
	FRChange(0, 0, 0),
	fRoPower(0.1f),
	isGround(false),
	JumpCnt(0),
	IsRepairEnable(false)
{

	//변신용 매시 생성==================================================================
	m_PlayerObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_PlayerObject->SetupAttachment(RootComponent);
		//설정
	m_PlayerObject->SetLinearDamping(0.15);
	m_PlayerObject->SetAngularDamping(3.0);
	m_PlayerObject->SetNotifyRigidBodyCollision(true);

	//사운드 가져오기===================================================================
	ConstructorHelpers::FObjectFinder<USoundWave> HeartBeatSoundAsset(TEXT("SoundWave'xxx/Game/Music/HeartBeat_Fast.HeartBeat_Fast'"));

	if (HeartBeatSoundAsset.Succeeded())
		SW_HeartBeat = HeartBeatSoundAsset.Object;

	ConstructorHelpers::FObjectFinder<USoundWave> ChaseSoundAsset(TEXT("SoundWave'xxx/Game/Music/HorrorChaseMusic.HorrorChaseMusic'"));

	if (ChaseSoundAsset.Succeeded())
		SW_Chase = ChaseSoundAsset.Object;

	//오디오 컴포넌트 부착
	AC_HeartBeat = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_HeartBeat"));
	AC_Chase = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Chase"));
	AC_HeartBeat->AttachTo(GetMesh());
	AC_Chase->AttachTo(GetMesh());

	//멀티플레이 리플리케이션 설정
	bReplicates = true;
	//GetMesh()->SetIsReplicated(true); //스켈레탈 매시
	//m_PlayerObject->SetIsReplicated(true); //사물 매시

	//머터리얼 가져오기===========================================================
	//캐릭터가 사용하는 매시입니다
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialHitAsset(TEXT("Material'/Game/Blueprints/MyMaterial/MT_MyHitRed.MT_MyHitRed'"));
	if (MaterialHitAsset.Succeeded())
		Setmat_hit_object(MaterialHitAsset.Object);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialHitAsset_human(TEXT("MaterialInstanceConstant'/Game/Blueprints/MyMaterial/MI_ColorPalette_Hit.MI_ColorPalette_Hit'"));
	if (MaterialHitAsset_human.Succeeded())
		Setmat_hit_human(MaterialHitAsset_human.Object);
}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &ASurvivor::Dash); //누를때 대시 실행
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &ASurvivor::DashStop); //대시 멈춤

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);

	//사물폼 변신 
	PlayerInputComponent->BindAction(TEXT("Change"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToObject);

	//인간폼 변신
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToPlayer);
}

void ASurvivor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{

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
	if (m_state != EPLAYER_STATE::OBJECT) {
		//상태 전환 (기계 파괴, 기계 수리)
		if(IsRepairEnable)
			ChangeState(EPLAYER_STATE::MACHINE);
	}

}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();

	//======플레이어 (초기) 데이터 설정==================================================
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		SetInfo(*(GI->GetSurvivorInfo(TEXT("Survivor1"))));
	}

	//플레이어 컨트롤러 가져오기
	PC = Cast<AMyPlayerController>(GetController());

	//킬러 가져오기 =====================================================================
	//월드 상의 특정 클래스 Actor을 가져오기
	TArray<AActor*> arrActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKiller::StaticClass(), arrActor);
	for (int i = 0; i < arrActor.Num(); i++) {
		arrKiller.Add(Cast<AKiller>(arrActor[i])); //형변환
		////크기 설정하기
		//SetKillerScale_Server(Cast<AKiller>(arrActor[i]),1.2f);
		
	}

	//사운드 스폰하기
	//AC_HeartBeat = UGameplayStatics::SpawnSound2D(this, SW_HeartBeat);
	AC_HeartBeat->SetSound(SW_HeartBeat);
	//AC_Chase = UGameplayStatics::SpawnSound2D(this, SW_Chase);
	AC_Chase->SetSound(SW_Chase);
	AC_HeartBeat->Stop();
	AC_Chase->Stop();
}

////안됨
//void ASurvivor::SetKillerScale_Server_Implementation(AKiller* killer,float f) {
//	SetKillerScale_Multicast(killer,f);
//}
//
//void ASurvivor::SetKillerScale_Multicast_Implementation(AKiller* killer,float f) {
//	if (killer) {
//		killer->GetMesh()->SetRelativeScale3D(FVector(f, f, f));
//		UE_LOG(LogTemp, Log, TEXT("set sets etset"));
//	}
//}

void ASurvivor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//레이캐스트로 변신가능 오브젝트 판별======================================================
	SelectObject();

	//======================================================================================

	//스태미너 업데이트
	UpdateSP();

	//대시 && 스태미너 사용 && 회복
	UseSP();

	//======================================================================================

	//살인마와 거리 차이에 따라서 비네팅 효과 & 화면 그레인 (지터) 증가 감소
	ShowVinetting();

	//발전기 UI 업데이트=====================================================================
	if (PC) {
		int machineNum = PC->GetDoneMachineNum();
		//이미지 변경하기
		if (machineNum > 0)
			PC->GetMainHUD()->GetTimerHUD()->SetMachineImge_Done(machineNum - 1);
	}


	//죽기==================================================================================
	if (GetInfo()->fCurHP <= 0) {
		
		ChangeState(EPLAYER_STATE::DEAD);
		if (isObject) {
			ChangeToPlayer(); //사물폼이면 인간형으로 바뀌고 죽기 
		}

	}

	//발전기가 모두 수리되면     ===========================================================
	if (PC) {
		if (PC->GetIsAllMachineRepaired()) {
			SetIsRepairEnable(false);
		}
	}

}

void ASurvivor::UpdateSP_Implementation() {

	UpdateSP_Client();
}

void ASurvivor::ShowVinetting_Implementation() {

	ShowVinetting_Client();
}

void ASurvivor::UpdateSP_Client_Implementation() {
	//체력, 스테미나 실시간 업데이트
	if (m_PrevHP != m_Info.fCurHP || m_PrevSP != m_Info.fCurSP)
	{
		// 이전 체력과 현제 체력이 다르다.
		// HUD 갱신
		float HPRatio = m_Info.fCurHP / m_Info.fMaxHP;
		float SPRatio = m_Info.fCurSP / m_Info.fMaxSP;

		if (PC != nullptr) PC->UpdatePlayHUD_Survivor(HPRatio, SPRatio, m_Info.fCurHP, m_Info.fMaxHP);

	}

	// 현재 체력, 스태미너를 이전프레임 체력으로 갱신
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;
}


void ASurvivor::ShowVinetting_Client_Implementation(){
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
			if (m_Cam->PostProcessSettings.VignetteIntensity < 0) m_Cam->PostProcessSettings.VignetteIntensity = 0;

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
}


void ASurvivor::UseSP/*_Client_Implementation*/() {
	//대시 가능 여부)
	if (GetInfo()->fCurSP < 0.1) {
		SetisDashEnable_Server(false);
		//isDashEnable = false;
	}
	//UE_LOG(LogTemp, Log, TEXT("isDashEnable : %i"), isDashEnable);
	//UE_LOG(LogTemp, Log, TEXT("isMoving : %i"), isMoving);
	//UE_LOG(LogTemp, Log, TEXT("isDashPressed : %i"), isDashPressed);
	//UE_LOG(LogTemp, Log, TEXT("isDashed : %i"), isDashed);

	//대시 중)
	//[1] 스태미너 감소
	if (isDashEnable && isDashPressed && isMoving) {
		//GetCharacterMovement()->MaxWalkSpeed = 1200.f; //속도 증가 (대시)
		////DashSpeedUp_Server();
		ChangeState(EPLAYER_STATE::DASH); //상태 전환
		//isDashed = true;
		SetisDashed_Server(true);

		GetInfo()->fCurSP -= 0.4f;
		if (GetInfo()->fCurSP < 0) GetInfo()->fCurSP = 0;
	}
	//[2] 대시 아닌 상태) 스태미너 회복
	else {
		//GetCharacterMovement()->MaxWalkSpeed = 600.f; //속도 원상복귀
		////DashSpeedDown_Server();

		//UE_LOG(LogTemp, Log, TEXT("isDashEnable : %i"), isDashEnable);
		//UE_LOG(LogTemp, Log, TEXT("isMoving : %i"), isMoving);
		//UE_LOG(LogTemp, Log, TEXT("isDashPressed : %i"), isDashPressed);
		//UE_LOG(LogTemp, Log, TEXT("isDashed : %i"), isDashed);

		SetisDashed_Server(false);
		//isDashed = false;

		GetInfo()->fCurSP += 0.035f;
		if (GetInfo()->fCurSP > GetInfo()->fMaxSP) GetInfo()->fCurSP = GetInfo()->fMaxSP;
	}

	//대쉬상태일때 속력 증가
	if (isDashed) {
		if (GetCharacterMovement()->MaxWalkSpeed != 1200.f) DashSpeedUp_Server();
	}
	else {
		if (GetCharacterMovement()->MaxWalkSpeed != 600.f) DashSpeedDown_Server();
	}

}

void ASurvivor::DashSpeedUp_Server_Implementation(){
	DashSpeedUp_Multicast();
}
void ASurvivor::DashSpeedUp_Multicast_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	UE_LOG(LogTemp, Log, TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ASurvivor::DashSpeedDown_Server_Implementation(){
	DashSpeedDown_Multicast();
}
void ASurvivor::DashSpeedDown_Multicast_Implementation(){
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	UE_LOG(LogTemp, Log, TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ASurvivor::TurnRed() {

	if (isObject) { //사물
		Setmat_original(m_PlayerObject->GetMaterial(0));
		//오리지날 메테리얼을 저장하고 레드 머터리얼로 바꿈
		if (Getmat_hit_object())
			m_PlayerObject->SetMaterial(0, Getmat_hit_object());
	}
	else { //사람
		Setmat_original(GetMesh()->GetMaterial(0));
		//오리지날 메테리얼을 저장하고 레드 머터리얼로 바꿈
		if (Getmat_hit_human()) {
			GetMesh()->SetMaterial(0, Getmat_hit_human());
		}
			
	}

}
void ASurvivor::TurnOriginalColor() {

	if (Getmat_original()) {
		if (isObject) {
			m_PlayerObject->SetMaterial(0, Getmat_original());
		}
		else {
			GetMesh()->SetMaterial(0, Getmat_original());
		}
		Setmat_original(nullptr);
	}

}


void ASurvivor::HitColorReaction_Server_Implementation() //빨개졌다 다시 돌아옴
{
	HitColorReaction_Multicast();
}
void ASurvivor::HitColorReaction_Multicast_Implementation() //빨개졌다 다시 돌아옴
{
	TurnRed();
	GetWorld()->GetTimerManager().SetTimer(FHitRedTimer, this, &ASurvivor::TurnOriginalColor, 0.5f, false);
}

void ASurvivor::TurnMove() {
	if ((!isDashed && !isJumping) && m_state != EPLAYER_STATE::HIT) ChangeState(EPLAYER_STATE::MOVE);
}

//=======================

void ASurvivor::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
}

void ASurvivor::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
}

void ASurvivor::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{
}
