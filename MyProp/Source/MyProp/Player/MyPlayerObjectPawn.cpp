// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObjectPawn.h"
#include "MyCharacter.h"

// Sets default values
AMyPlayerObjectPawn::AMyPlayerObjectPawn()
	: fRunPower(5),
	fRoPower(0.1f),
	fJumpPower(500)
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
}

//void AMyPlayerObjectPawn::SetSpawnPos(FVector new_originalPos) {
//	originalPos = new_originalPos;
//}

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
	if (m_ObjectMesh->IsSimulatingPhysics()) {
		m_ObjectMesh->AddImpulse(FVector(0, 0, fJumpPower), NAME_None, true);
		UE_LOG(LogTemp, Log, TEXT("Jump!"));
	}
}

void AMyPlayerObjectPawn::Interaction() {

}

void AMyPlayerObjectPawn::PlayerObject() {

	//변신 해제하기 (인간 폰으로 돌아가기)

	if (nullptr != pCharacter)
	{
		GetWorld()->GetFirstPlayerController()->Possess(pCharacter);

		FVector originalPos = m_ObjectMesh->GetRelativeLocation(); //인간폼 돌아가기 당시 원래 [메시의] 위치 저장

		//물리 끄기
		m_ObjectMesh->SetSimulatePhysics(false);


		//원래 [인간형] 오브젝트 다른 곳으로 치우기 && 각도 시작과 같이 설정하기
		SetActorLocation(FVector(0, 0, 0));
		SetActorRotation(FRotator(0, 0, 0));
		//매시의 각도 초기화 하기
		m_ObjectMesh->SetAllPhysicsRotation(FRotator(0, 0, 0));
		//[메시] 부분 이동
		m_ObjectMesh->SetRelativeLocation(FVector(0, 0,
			m_ObjectMesh->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//인간폼의 위치를 오브젝트가 있던 액터 위치로 설정하기
		pCharacter->SetActorLocation(originalPos);

	}
}