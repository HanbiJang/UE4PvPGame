// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Anim/MyAnimInstance.h"

#include "Engine/BlueprintGeneratedClass.h"
#include "MyPlayerObjectPawn.h"

#include <MyProp/Player/Survivor/Survivor.h>

// Sets default values
AMyCharacter::AMyCharacter() :
	isMoving(false),
	isDashed(false),
	isDashPressed(false),
	isDashEnable(true),
	isJumping(false),
	m_state(EPLAYER_STATE::IDLE),
	isObject(false)
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
	//m_Arm->TargetArmLength = 800.f;
	//m_Arm->TargetOffset = FVector(300, 0, 500);
	//m_Cam->SetWorldRotation(FRotator(-50, 0, 0));
	m_Arm->TargetArmLength = 300.f;
	m_Arm->TargetOffset = FVector(0, 0, 40);
	m_Cam->SetupAttachment(m_Arm, USpringArmComponent::SocketName); //카메라암에 카메라 붙이기

	m_Arm->bUsePawnControlRotation = true;
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
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Moving
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"),this, &AMyCharacter::LeftRight);


	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::MyJump);

	//ItemBtn
	PlayerInputComponent->BindAction(TEXT("Item1"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item1);
	PlayerInputComponent->BindAction(TEXT("Item2"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item2);

	//camera
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);

	//상호작용
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AMyCharacter::Interaction);

	// 살인마 (공격 스킬 추가)
	// 생존자 (Interaction 추가)

}

void AMyCharacter::Turn(float f) {
	AddControllerYawInput(f);
}

void AMyCharacter::LookUp(float f) {
	AddControllerPitchInput(f);
}

void AMyCharacter::UpDown(float f) {
	fUpdown = f;

	// 위아래로 이동
	if (!isObject) {
		if (m_state == EPLAYER_STATE::CATCH || m_state == EPLAYER_STATE::DASH
			|| (m_state == EPLAYER_STATE::HIT && Cast<ASurvivor>(this)) //서바이벌만
			|| m_state == EPLAYER_STATE::IDLE || m_state == EPLAYER_STATE::JUMP
			|| m_state == EPLAYER_STATE::MOVE) {
			if (f != 0.f) {
				//대시나 점프일때 애니메이션 = 점프여야함 Move면 안됨
				TurnMove();
				//if ((!isDashed && !isJumping) 
				//	||m_state != EPLAYER_STATE::HIT) ChangeState(EPLAYER_STATE::MOVE);
				SetisMoving_Server(true);

				//캐릭터 회전과 이동
				FRotator r = FRotator(0, GetControlRotation().Yaw, 0);
				//GetForwardVector: r만큼 월드 정방향 벡터를 회전
				AddMovementInput(UKismetMathLibrary::GetForwardVector(r), f);
			}
			else if (fUpdown == 0 && fLeftRight == 0 && !isJumping && !isMoving) {
				if (m_state != EPLAYER_STATE::IDLE && m_state != EPLAYER_STATE::HIT) {
					UE_LOG(LogTemp, Log, TEXT("state idle change"));
					SetisMoving_Server(false);
					ChangeState(EPLAYER_STATE::IDLE);
				}
			}
		}
	}

}

void AMyCharacter::LeftRight(float f) {

	fLeftRight = f;
	//오른쪽, 왼쪽으로 이동
	if (!isObject) {
		if (m_state == EPLAYER_STATE::CATCH || m_state == EPLAYER_STATE::DASH
			|| (m_state == EPLAYER_STATE::HIT && Cast<ASurvivor>(this)) //서바이벌만
			|| m_state == EPLAYER_STATE::IDLE || m_state == EPLAYER_STATE::JUMP
			|| m_state == EPLAYER_STATE::MOVE) {
			if (f != 0.f) {
				//대시나 점프일때 애니메이션 = 점프여야함 Move면 안됨
				TurnMove();
				//if ((!isDashed && !isJumping) || m_state != EPLAYER_STATE::HIT) ChangeState(EPLAYER_STATE::MOVE);
				SetisMoving_Server(true);
				//isMoving = true;

				//캐릭터 회전과 이동
				FRotator r = FRotator(0, GetControlRotation().Yaw, 0);
				//GetForwardVector: r만큼 월드 정방향 벡터를 회전
				AddMovementInput(UKismetMathLibrary::GetRightVector(r), f);
			}

			else if (fUpdown == 0 && fLeftRight == 0 && !isJumping) {
				if (m_state != EPLAYER_STATE::IDLE && m_state != EPLAYER_STATE::HIT) {
					UE_LOG(LogTemp, Log, TEXT("state idle change"));
					SetisMoving_Server(false);
					ChangeState(EPLAYER_STATE::IDLE);
				}
			}
		}
	}
}

void AMyCharacter::MyJump() {
	if ( (m_state != EPLAYER_STATE::OBJECT && m_state != EPLAYER_STATE::DASH)
		 || (m_state == EPLAYER_STATE::HIT && Cast<ASurvivor>(this)) //서바이벌만
		) {
		isJumping = true;
		UE_LOG(LogTemp, Log, TEXT("Jump"))
		ChangeState(EPLAYER_STATE::JUMP);
	}
}

void AMyCharacter::JumpAction() {
	ACharacter::Jump();
}

void AMyCharacter::Dash()
{

}

void AMyCharacter::DashStop()
{

}

void AMyCharacter::Interaction()
{
	if (m_state != EPLAYER_STATE::OBJECT) {
		//상태 전환 (기계 파괴, 기계 수리)
		ChangeState(EPLAYER_STATE::MACHINE);
	}

}



void AMyCharacter::Item1() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item1 used"));
}

void AMyCharacter::Item2() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item2 used"));
}

void AMyCharacter::ChangeState_Implementation(EPLAYER_STATE newState)
{
	//if (m_state == newState) {
	//	return;
	//}
	////else
	//m_state = newState;
	//UE_LOG(LogTemp, Log, TEXT("state: %i"), m_state);
	ChangeState_Multicast(newState);
}

void AMyCharacter::ChangeState_Multicast_Implementation(EPLAYER_STATE newState) {

	if (m_state == newState) {
		return;
	}
	//else
	//점프에서 Idle로 돌아올 시 점프중이 아님을 알림
	if(m_state == EPLAYER_STATE::JUMP && newState == EPLAYER_STATE::IDLE) isJumping = false;
	m_state = newState;
	UE_LOG(LogTemp, Log, TEXT("state: %i"), m_state);

}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//캐릭터 상태를 리플리케이트한다
	DOREPLIFETIME(AMyCharacter, m_state);

	//캐릭터 이동 멈춤 (Idle 상태로 바꾸기 위한)
	DOREPLIFETIME(AMyCharacter, isMoving);
	DOREPLIFETIME(AMyCharacter, fLeftRight);
	DOREPLIFETIME(AMyCharacter, fUpdown);
	DOREPLIFETIME(AMyCharacter, isJumping);

	DOREPLIFETIME(AMyCharacter, isDashed);
	DOREPLIFETIME(AMyCharacter, isDashPressed);
	DOREPLIFETIME(AMyCharacter, isDashEnable);

	DOREPLIFETIME(AMyCharacter, isObject);
}

//state값이 바뀌면 호출됨
void AMyCharacter::OnRep_State() {

	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString stateMessage = FString::Printf(TEXT("Survivor State Changed: %i"), m_state);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, stateMessage);
	}

	//Server-specific functionality
	//if (Role == ROLE_Authority)
	if (HasAuthority())
	{
		FString stateMessage = FString::Printf(TEXT("Killer State Changed: %i"), m_state);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, stateMessage);
	}

	//서버, 클라이언트 공통 부분

}

//대시

void AMyCharacter::SetisDashEnable_Server_Implementation(bool value) {
	SetisDashEnable_Multicast(value);
}

void AMyCharacter::SetisDashEnable_Multicast_Implementation(bool value) {
	isDashEnable = value;
}

void AMyCharacter::SetisDashPressed_Server_Implementation(bool value) {
	SetisDashPressed_Multicast(value);
}

void AMyCharacter::SetisDashPressed_Multicast_Implementation(bool value) {
	isDashPressed = value;
}

//값 바꾸기(서버)
void AMyCharacter::SetisDashed_Server_Implementation(bool value) {
	SetisDashed_Multicast(value);
}

//값 바꾸기(멀티캐스트)
void AMyCharacter::SetisDashed_Multicast_Implementation(bool value) {
	isDashed = value;
}

//값 바꾸기(서버)
void AMyCharacter::SetisMoving_Server_Implementation(bool value) {
	SetisMoving_Multicast(value);
}

//값 바꾸기(멀티캐스트)
void AMyCharacter::SetisMoving_Multicast_Implementation(bool value) {
	isMoving = value;
}