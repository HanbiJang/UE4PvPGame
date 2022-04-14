// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Anim/MyAnimInstance.h"

#include "Engine/BlueprintGeneratedClass.h"
#include "MyPlayerObjectPawn.h"

// Sets default values
AMyCharacter::AMyCharacter() :
	isDashed(false),
	m_state(EPLAYER_STATE::IDLE),
	FVChange(0, 0, 0),
	FRChange(0, 0, 0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//매시 설정
		//더미 (남성) 스킨:
		//SkeletalMesh'/Game/PolygonPrototype/Meshes/Characters/SK_Character_Dummy_Male_01.SK_Character_Dummy_Male_01'
		
	//CreateDefaultSubObject 원하는 컴포넌트를 생성함	

	//Mesh 레퍼런스 경로로 찾아오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMesh(
		TEXT("SkeletalMesh'/Game/PolygonPrototype/Meshes/Characters/SK_Character_Dummy_Male_01.SK_Character_Dummy_Male_01'"));

	if (BodyMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BodyMesh.Object);
		
	}

	//카메라 설정
	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_Arm->SetupAttachment(RootComponent); //컴포넌트에 카메라 붙이기

	//카메라 위치 세팅
	m_Arm->TargetArmLength = 800.f;
	m_Arm->TargetOffset = FVector(300, 0, 500);
	m_Cam->SetWorldRotation(FRotator(-50, 0, 0));

	m_Arm->bUsePawnControlRotation = true;

	m_Cam->SetupAttachment(m_Arm, USpringArmComponent::SocketName); //카메라암에 카메라 붙이기

	m_Cam->bUsePawnControlRotation = false;
	

	//3인칭 회전 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	//애니메이션 설정
	m_AnimInst = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance()); //캐릭터의 매시에서 설정된 AnimInstance 클래스 가져오기

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//변신용 사물 오브젝트
	//블.프 오브젝트 스폰
	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(),
		TEXT("Blueprint'/Game/Blueprints/Objects/BP_MyPlayerObjectPawn.BP_MyPlayerObjectPawn_C'"));

	if (LoadBP)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PlayerObjectPawn = nullptr;

		m_PlayerObjectPawn = GetWorld()->SpawnActor<AMyPlayerObjectPawn>(LoadBP,
			FVector(0, 0, 0),//물체의 대략적 크기 구하기
			FRotator(0, 0, 0),
			SpawnInfo);
		m_PlayerObjectPawn->SetPCharacter(this); //인간폼 정보등록
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//멈춤
	if (fLeftRight == 0.f && fUpdown == 0.f && m_state == EPLAYER_STATE::MOVE) {
		ChangeState(EPLAYER_STATE::IDLE);
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Moving
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Repeat, this, &AMyCharacter::Dash); //누르는 동안 대시 실행
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &AMyCharacter::DashStop); //대시 멈춤
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"),this, &AMyCharacter::LeftRight);


	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	//ItemBtn
	PlayerInputComponent->BindAction(TEXT("Item1"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item1);
	PlayerInputComponent->BindAction(TEXT("Item2"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item2);


	//상호작용
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AMyCharacter::Interaction);

	// 살인마 (공격 스킬 추가)
	// 생존자 (Interaction 추가)

}

void AMyCharacter::UpDown(float f) {
	// 위아래로 이동
	if ( f != 0.f && m_state != EPLAYER_STATE::JUMP && m_state != EPLAYER_STATE::DASH ) {
		ChangeState(EPLAYER_STATE::MOVE);
	}

	fLeftRight = f;
		
	//캐릭터 회전과 이동
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, f);
}

void AMyCharacter::LeftRight(float f) {
	//오른쪽, 왼쪽으로 이동
	if (f != 0.f && m_state != EPLAYER_STATE::JUMP && m_state != EPLAYER_STATE::DASH && !isDashed) {
		ChangeState(EPLAYER_STATE::MOVE);
	}

	fUpdown = f;

	//캐릭터 회전과 이동
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, f);
}

void AMyCharacter::Dash()
{
	isDashed = true;

	if (m_state != EPLAYER_STATE::OBJECT) {
		//속도 증가 (대시)
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;

		//상태 전환
		ChangeState(EPLAYER_STATE::DASH);
	}
}

void AMyCharacter::DashStop()
{
	isDashed = false;

	if (m_state != EPLAYER_STATE::OBJECT) {
		//속도 원상복귀
		GetCharacterMovement()->MaxWalkSpeed = 600.f;

		//상태 전환
		ChangeState(EPLAYER_STATE::MOVE);
	}
}

void AMyCharacter::Interaction()
{
	if (m_state != EPLAYER_STATE::OBJECT) {
		//상태 전환 (기계 파괴, 기계 수리)
		ChangeState(EPLAYER_STATE::MACHINE);
	}

}

void AMyCharacter::Jump() {
	if (m_state != EPLAYER_STATE::DASH) {
		UE_LOG(LogTemp, Log, TEXT("Jump"))
		ChangeState(EPLAYER_STATE::JUMP);
	}

	ACharacter::Jump();

}

void AMyCharacter::JumpAction() {
	ACharacter::Jump();
}

void AMyCharacter::Item1() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item1 used"));
}

void AMyCharacter::Item2() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item2 used"));
}

void AMyCharacter::ChangeState(EPLAYER_STATE newState)
{
	if (m_state == newState) {
		return;
	}

	//else

	m_state = newState;
	UE_LOG(LogTemp, Log, TEXT("state: %i"), m_state);
}

void AMyCharacter::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//플레이어 상태 변경
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //변신 당시 원래 위치 저장

	//원래 [인간형] 오브젝트 다른 곳으로 치우기 && 각도 시작과 같이 설정하기
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//물리 켜기
	m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(true);

	//물체 위치를 인간폼이 있던 곳으로 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));

	//클릭한 오브젝트 메시의 크기와 같게 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetRelativeScale3D(fscale);
	//매시 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetStaticMesh(mesh);

	//시점 옮기기...
	GetWorld()->GetFirstPlayerController()->Possess(m_PlayerObjectPawn);

	//변신 가능 상태 조절
	//1.5초 뒤에 변신 가능해지기
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FChangeEnableTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);
}


