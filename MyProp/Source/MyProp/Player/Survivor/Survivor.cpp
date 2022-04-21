// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
//#include "../MyPlayerObjectPawn.h"


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
}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);

	//인간폼 변신
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToPlayer);
}

void ASurvivor::UpDown(float f) {
	Super::UpDown(f);

	if(m_state == EPLAYER_STATE::OBJECT){
		if (f != 0 && m_PlayerObject->IsSimulatingPhysics()) {
			m_PlayerObject->AddImpulse(FVector(fRunPower * f, 0, 0), NAME_None, true);
			m_PlayerObject->AddAngularImpulseInDegrees(FVector(0, fRoPower * f, 0.f), NAME_None, true);
		}
	}
}
void ASurvivor::LeftRight(float f) {
	Super::LeftRight(f);

	if (m_state == EPLAYER_STATE::OBJECT) {
		if (f != 0 && m_PlayerObject->IsSimulatingPhysics()) {
			m_PlayerObject->AddImpulse(FVector(0, fRunPower * f, 0.f), NAME_None, true);
			m_PlayerObject->AddAngularImpulseInDegrees(FVector(fRoPower * f, 0, 0.f), NAME_None, true);
		}
	}
}
void ASurvivor::Jump() {
	Super::Jump();

	if (m_state == EPLAYER_STATE::OBJECT) {
		if (JumpCnt < 2 && m_PlayerObject->IsSimulatingPhysics()) {
			JumpCnt++;
			isGround = false;
			m_PlayerObject->AddImpulse(FVector(0, 0, fJumpPower), NAME_None, true);
			UE_LOG(LogTemp, Log, TEXT("Jump!"));
		}
	}
}

void ASurvivor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Jump? : %i"), isGround);
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

	////======블레이어 변신용 사물 오브젝트 블루프린트======
	////가져와서 스폰시키기
	//UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(),
	//	TEXT("Blueprint'/Game/Blueprints/Survivor/BP_MyPlayerObjectPawn.BP_MyPlayerObjectPawn_C'"));
	//if (LoadBP)
	//{
	//	FActorSpawnParameters SpawnInfo;
	//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	m_PlayerObjectPawn = nullptr;
	//	m_PlayerObjectPawn = GetWorld()->SpawnActor<AMyPlayerObjectPawn>(LoadBP,
	//		FVector(0, 0, 0),//물체의 대략적 크기 구하기
	//		FRotator(0, 0, 0),
	//		SpawnInfo);
	//	m_PlayerObjectPawn->SetPCharacter(this); //인간폼 정보등록
	//}

	//킬러 가져오기
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

void ASurvivor::Dash()
{
	UE_LOG(LogTemp, Log, TEXT("Dasu!!!"));
	isDashPressed = true;

	if (m_state != EPLAYER_STATE::OBJECT && GetInfo()->fCurSP > 10) {
		isDashEnable = true;
	}
}

void ASurvivor::DashStop()
{
	isDashPressed = false;
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
				UE_LOG(LogTemp, Log, TEXT("Killer Name!!! : %s"), *(arrKiller[i]->GetName()));
				UE_LOG(LogTemp, Log, TEXT("Killer distance!!! : %f"), GetDistanceTo(arrKiller[i]));

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

}

//거리구하기 함수...
float ASurvivor::MyGetDistance(FVector a, FVector b) {
	float res = sqrtf(powf(a.X - b.X, 2) + powf(a.Y - b.Y, 2) + powf(a.Z - b.Z, 2));
	return res;
}

//인간에서 사물로 변신
void ASurvivor::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//플레이어 상태 변경
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //변신 당시 원래 위치 저장

	//원래 [인간형] 오브젝트 다른 곳으로 치우기 && 각도 시작과 같이 설정하기
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//물리 끄기
	//m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(false);
	m_PlayerObject->SetSimulatePhysics(false);

	//물체 위치를 인간폼이 있던 곳으로 설정
	//m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));
	m_PlayerObject->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));


	//클릭한 오브젝트 메시의 크기와 같게 설정
	//m_PlayerObjectPawn->m_ObjectMesh->SetRelativeScale3D(fscale);
	//m_PlayerObjectPawn->m_ObjectMesh->SetStaticMesh(mesh);
	m_PlayerObject->SetRelativeScale3D(fscale);
	m_PlayerObject->SetStaticMesh(mesh);

	//시점 옮기기...
	//GetWorld()->GetFirstPlayerController()->Possess(m_PlayerObjectPawn);
	//카메라만 옮기기
	m_Arm->DetachFromParent(true);
	//m_Arm->SetupAttachment(m_PlayerObject);
	m_Arm->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);

	//0.1초 뒤에 오브젝트 물리 켜기
	//GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FPhysicsTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetSimulatePhysicsTrue, 0.1f, false);
	GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &ASurvivor::SetSimulatePhysicsTrue, 0.1f, false);

	//변신 가능 상태 조절
	//1.5초 뒤에 변신 가능해지기
	//GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FChangeEnableTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);
	GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);

	//캐릭터 정보 옮기기
	//m_PlayerObjectPawn->SetInfo(m_Info);

	//변신 시 캐릭터 체력 디버프
	//[미구현]
}

//사물에서 새 사물로 변신
void ASurvivor::ChangeObjectMesh(UStaticMesh* mesh, FVector scale)
{
	if (bChangeEnable) {
		bChangeEnable = false; //과다변신 막기

		//물리 끄기
		m_PlayerObject->SetSimulatePhysics(false);

		//매시의 각도 초기화 하기
		m_PlayerObject->SetAllPhysicsRotation(FRChange);
		//[매시] 부분 이동
		m_PlayerObject->SetRelativeLocation(m_PlayerObject->GetRelativeLocation() + FVector(0, 0,
			m_PlayerObject->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//매시 변경
		m_PlayerObject->SetStaticMesh(mesh);
		//크기 설정
		m_PlayerObject->SetRelativeScale3D(scale);

		//0.1초 뒤에 물리 켜기
		GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &ASurvivor::SetSimulatePhysicsTrue, 0.1f, false);

		//1.5초 뒤에 변신 가능해지기
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);
	}
}

//사물에서 사람으로 변신
void ASurvivor::ChangeToPlayer() {

	if (bChangeEnable) {
		bChangeEnable = false; //과다변신 막기

		//시점 변경
		m_Arm->DetachFromParent(true);
		m_Arm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		m_Arm->SetRelativeLocation(FVector(0, 0, 0));

		FVector originalPos = m_PlayerObject->GetRelativeLocation(); //인간폼 돌아가기 당시 원래 [메시의] 위치 저장

		m_PlayerObject->SetSimulatePhysics(false); //물리 끄기

		//매시의 각도 초기화 하기
		m_PlayerObject->SetAllPhysicsRotation(FRChange);
		//[메시] 부분 이동
		m_PlayerObject->SetRelativeLocation(FVChange 
			+ FVector(0, 0, m_PlayerObject->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//인간폼의 위치를 오브젝트가 있던 액터 위치로 설정하기
		SetActorLocation(originalPos);
		////인간폼의 각도를 사물의 z? 각도와 일치시키기
		//SetActorRotation(FRotator(0,m_PlayerObject->GetRelativeRotation().Yaw,0));

		//1.5초 뒤에 변신 가능해지기
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);
		
		//상태 변화
		ChangeState(EPLAYER_STATE::IDLE);
	}
}



