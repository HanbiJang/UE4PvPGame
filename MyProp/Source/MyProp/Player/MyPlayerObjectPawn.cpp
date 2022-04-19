// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObjectPawn.h"
//#include "MyCharacter.h"
#include "Survivor/Survivor.h"

// Sets default values
AMyPlayerObjectPawn::AMyPlayerObjectPawn()
	: fRunPower(5),
	fRoPower(0.1f),
	fJumpPower(500),
	FVChange(0,0,0),
	FRChange(0,0,0),
	bChangeEnable(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//캡슐 컴포넌트	
	RootComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	//변신 용 메시
	m_ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_ObjectMesh->SetupAttachment(RootComponent);

	//카메라 설정
	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_Arm->SetupAttachment(m_ObjectMesh); //매시에 카메라 붙이기

	m_Arm->bUsePawnControlRotation = true;

	m_Cam->SetupAttachment(m_Arm, USpringArmComponent::SocketName); //카메라암에 카메라 붙이기

	//카메라 위치 세팅
	m_Arm->TargetArmLength = 800.f;
	m_Arm->TargetOffset = FVector(300, 0, 500);
	m_Cam->SetWorldRotation(FRotator(-50, 0, 0));

	m_Cam->bUsePawnControlRotation = false;

	//3인칭 회전 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AMyPlayerObjectPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayerObjectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////컴포넌트 위치를 Physics Location으로 업데이트
	//RootComponent->SetWorldLocation(m_ObjectMesh->GetComponentLocation());
}

// Called to bind functionality to input
void AMyPlayerObjectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Moving
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPlayerObjectPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPlayerObjectPawn::LeftRight);

	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayerObjectPawn::Jump);

	//상호작용
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AMyPlayerObjectPawn::Interaction);

	//인간폼 변신
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &AMyPlayerObjectPawn::PlayerObject);
}

void AMyPlayerObjectPawn::UpDown(float f)
{
	if (f != 0 && m_ObjectMesh->IsSimulatingPhysics()) {
		m_ObjectMesh->AddImpulse(FVector(fRunPower* f, 0, 0), NAME_None, true);
		m_ObjectMesh->AddAngularImpulseInDegrees(FVector(0, fRoPower *f, 0.f), NAME_None, true);
	}

}

void AMyPlayerObjectPawn::LeftRight(float f)
{
	if (f != 0 && m_ObjectMesh->IsSimulatingPhysics()) {
		m_ObjectMesh->AddImpulse(FVector(0, fRunPower * f, 0.f), NAME_None, true);
		m_ObjectMesh->AddAngularImpulseInDegrees(FVector(fRoPower *f, 0, 0.f), NAME_None, true);
	}
}

void AMyPlayerObjectPawn::Jump()
{
	if (JumpCnt < 2 && m_ObjectMesh->IsSimulatingPhysics()) {
		JumpCnt++;
		isGround = false;
		m_ObjectMesh->AddImpulse(FVector(0, 0, fJumpPower), NAME_None, true);
		UE_LOG(LogTemp, Log, TEXT("Jump!"));
	}
}

void AMyPlayerObjectPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//벽은 ground로 치지 않기
	//+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Block,bTraceType=False,bStaticObject=False,Name="WallObject")
	if (OtherComp->GetCollisionObjectType() != ECC_GameTraceChannel2) {
		if (!isGround) { 
			isGround = true; 
			JumpCnt = 0;
		}
	}
	

}

//재변신 (연속 변신)
void AMyPlayerObjectPawn::ChangeObjectMesh(UStaticMesh* mesh, FVector scale)
{
	if (bChangeEnable) {
		bChangeEnable = false; //과다변신 막기

		//물리 끄기
		m_ObjectMesh->SetSimulatePhysics(false);

		//매시의 각도 초기화 하기
		m_ObjectMesh->SetAllPhysicsRotation(FRChange);
		//[매시] 부분 이동
		m_ObjectMesh->SetRelativeLocation(m_ObjectMesh->GetRelativeLocation() + FVector(0, 0,
			m_ObjectMesh->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//매시 변경
		m_ObjectMesh->SetStaticMesh(mesh);
		//크기 설정
		m_ObjectMesh->SetRelativeScale3D(scale);

		//0.1초 뒤에 물리 켜기
		GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &AMyPlayerObjectPawn::SetSimulatePhysicsTrue, 0.1f, false);

		//1.5초 뒤에 변신 가능해지기
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("bChangeEnable false!"));
	}
}

void AMyPlayerObjectPawn::Interaction() {

}

/// <summary>
/// 변신 해제하기 (인간 폰으로 돌아가기)
/// </summary>
void AMyPlayerObjectPawn::PlayerObject() {

	if (bChangeEnable) {

		if (nullptr != pCharacter) //외부에서 정보 등록해줌
		{
			bChangeEnable = false; //과다변신 막기
			GetWorld()->GetFirstPlayerController()->Possess(pCharacter);

			FVector originalPos = m_ObjectMesh->GetRelativeLocation(); //인간폼 돌아가기 당시 원래 [메시의] 위치 저장

			//물리 끄기
			m_ObjectMesh->SetSimulatePhysics(false);


			//원래 [인간형] 오브젝트 다른 곳으로 치우기 && 각도 시작과 같이 설정하기
			SetActorLocation(FVChange);
			SetActorRotation(FRChange);
			//매시의 각도 초기화 하기
			m_ObjectMesh->SetAllPhysicsRotation(FRChange);
			//[메시] 부분 이동
			m_ObjectMesh->SetRelativeLocation(FVChange + FVector(0, 0,
				m_ObjectMesh->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

			//인간폼의 위치를 오브젝트가 있던 액터 위치로 설정하기
			pCharacter->SetActorLocation(originalPos);

			//1.5초 뒤에 변신 가능해지기
			GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);

			//캐릭터 정보 옮기기
			pCharacter->SetInfo(m_Info);
		}
	}
}