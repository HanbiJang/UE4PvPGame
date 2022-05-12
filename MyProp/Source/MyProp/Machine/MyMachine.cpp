// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMachine.h"
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/Controller/MyPlayerController.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/Mode/MyPropGameModeBase.h>

// Sets default values
AMyMachine::AMyMachine()
:
IsEnable(true),
Range(100.f), //수리 거리
RepairTime(10.f), //총 수리해야하는 시간
CurRepairTime(0.f),
IsDone(false) //수리 완료 여부
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//캡슐 컴포넌트
	m_Cap = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = m_Cap;

	//매시
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);

	//박스 영역 생성
	RepairBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RepairBox"));
	RepairBox->SetupAttachment(m_Mesh);
	//박스 range 설정
	RepairBox->SetBoxExtent(FVector(Range, Range, Range));
	//오버랩 이벤트 설정
	RepairBox->SetGenerateOverlapEvents(true);

	//라이트
	m_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("DoneLight"));
	m_Light->SetupAttachment(m_Mesh);
	m_Light->SetIntensity(0.f);
	UE_LOG(LogTemp, Log, TEXT("Intensity : %f"), m_Light->Intensity);

	//다 진행됐을 때의 매시
	ConstructorHelpers::FObjectFinder<UStaticMesh> DoneMeshAsset(
		TEXT("StaticMesh'/Game/Blueprints/MyMesh/SM_PROXY_SM_press_box_Done.SM_PROXY_SM_press_box_Done'"));
	if (DoneMeshAsset.Succeeded())
		m_DoneMesh = DoneMeshAsset.Object;
}

// Called when the game starts or when spawned
void AMyMachine::BeginPlay()
{
	Super::BeginPlay();
	
	//오버랩 이벤트에서 검출하기
	RepairBox->OnComponentBeginOverlap.AddDynamic(this, &AMyMachine::OnBeginOverlap);
	RepairBox->OnComponentEndOverlap.AddDynamic(this, &AMyMachine::OnEndOverlap);

}

// Called every frame
void AMyMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDone && CurRepairTime >= RepairTime) {//수리 완료	
		IsDone = true; 
		m_Light->SetIntensity(10000.f);//라이트 켜기 15000.
		//매시 변경
		if(m_DoneMesh)
			m_Mesh->SetStaticMesh(m_DoneMesh);
		//생존자 상태 변경
		if(sur){
			sur->ChangeState(EPLAYER_STATE::IDLE);
		}

		//모든 게임 컨트롤러에서 타이머 시간, 발전기 개수 증가
		for (FConstControllerIterator itr = GetWorld()->GetControllerIterator(); itr; ++itr) {
			AMyPlayerController* PC = Cast<AMyPlayerController>(*itr);
			if (PC) {
				PC->SetMachineDone(true); //완료한 발전기 갯수 증가
				int machineNum = PC->GetDoneMachineNum();
			}
			//타이머 시간 증가
			//PC->SetGameLeftTimeSec(PC->GetGameLeftTimeSec() + 120.f);
			//AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(GetWorld()->GetMode);
			SetTimerTime_Server(120.f);
		}

		
	}

	if (!IsDone && IsEnable) { //수리 가능하면
		//수리중이면 시간 깎기
		if (sur && sur->IsRepairEnable && sur->GetState() == EPLAYER_STATE::MACHINE) {
			CurRepairTime = FMath::Clamp(CurRepairTime+DeltaTime, 0.f, RepairTime);
			//UE_LOG(LogTemp, Log, TEXT("Repair Time: %f"), CurRepairTime);
		}
		//발전기 범위 안에 있다면 UI 업데이트
		if (sur && sur->IsRepairEnable && !IsDone) {
			//들어온 생존자의 기계 수리 UI 업데이트 (완료 안됐을때만)
			AMyPlayerController* PC = Cast<AMyPlayerController>(sur->GetInstigatorController());
			if (PC) {
				float ratio = CurRepairTime / RepairTime;
				PC->UpdateMachineHUD_Survivor(ratio);
			}
				
		}

	}

}

void AMyMachine::SetTimerTime_Server_Implementation(float f) {
	
	//게임 모드에서 타이머 시간 증가
	AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(GetWorld()->GetAuthGameMode());
	GM->SetGameLeftTimeSec(GM->GetGameLeftTimeSec() + f); //f초 만큼 추가

}

void AMyMachine::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	//들어온 생존자의 수리 가능을 true 로
	sur = Cast<ASurvivor>(_OtherActor);
	if (sur != nullptr && !IsDone) {
		sur->SetIsRepairEnable(true);//근처에있음 (수리가능)
		MachineUIVisiblity_Client(true, sur);
	}

}

void AMyMachine::MachineUIVisiblity_Client_Implementation(bool b, ASurvivor* _sur) {

	//들어온 생존자의 기계 수리 UI띄우기
	if (_sur != nullptr) {
		AMyPlayerController* PC = Cast<AMyPlayerController>(_sur->GetController());
		if (PC != nullptr) {
			if (PC->GetMainHUD() && PC->GetMainHUD()->GetMachineHUD()) {
				if (b) {
					PC->GetMainHUD()->GetMachineHUD()->SetVisibility(ESlateVisibility::Visible);
				}

				else {
					PC->GetMainHUD()->GetMachineHUD()->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}

}

void AMyMachine::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
	//나간 생존자의 수리 가능을 false 로
	if (sur) {
		sur->SetIsRepairEnable(false);
		//나간 생존자의 기계 수리 UI없애기
		MachineUIVisiblity_Client(false, sur);

		sur = nullptr; //순서 바꾸지말아라
	}
	
}

void AMyMachine::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{

}



