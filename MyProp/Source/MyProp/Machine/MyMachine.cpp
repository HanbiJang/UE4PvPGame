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
RepairTime(5.f), //총 수리해야하는 시간
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

	if (!IsDone && CurRepairTime >= RepairTime) {
		//수리 완료	
		IsDone = true; 
		IsEnable = false;
		m_Light->SetIntensity(10000.f);//라이트 켜기 15000.
		//매시 변경
		if(m_DoneMesh)
			m_Mesh->SetStaticMesh(m_DoneMesh);
		//수리하던 생존자들 상태 변경
		for (int i = 0; i < surArr.Num(); i++) {
			surArr[i]->ChangeState(EPLAYER_STATE::IDLE);
		}
		//모든 생존자들 && 킬러의 UI 업데이트 관련
		SetMachineDoneAllPlayer_Server();
		
	}

	if (!IsDone && IsEnable) { //수리 가능하면

		for(int i=0; i<surArr.Num(); i++){
			//수리중이면 시간 깎기
			if (surArr[i] && surArr[i]->IsRepairEnable && surArr[i]->GetState() == EPLAYER_STATE::MACHINE) {
				CurRepairTime = FMath::Clamp(CurRepairTime + DeltaTime, 0.f, RepairTime);			
			}
			//발전기 범위 안에 있다면 UI 업데이트
			if (surArr[i] && surArr[i]->IsRepairEnable && !IsDone) {
				//들어온 생존자의 기계 수리 UI 업데이트 (완료 안됐을때만)
				AMyPlayerController* PC = Cast<AMyPlayerController>(surArr[i]->GetController());
				if (PC) {
					float ratio = CurRepairTime / RepairTime;
					PC->UpdateMachineHUD_Survivor(ratio);
				}

			}
		}

	}

}

void AMyMachine::SetMachineDoneAllPlayer_Server_Implementation(){
	SetMachineDoneAllPlayer_Multicast();
}

void AMyMachine::SetMachineDoneAllPlayer_Multicast_Implementation(){	
	//타이머 시간 증가 && 수리한 발전기 수 증가
	SetTimerUpdate_Server(120.f);
}

void AMyMachine::SetTimerUpdate_Server_Implementation(float f) {
	
	//게임 모드에서 타이머 시간 증가
	AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->SetGameLeftTimeSec(GM->GetGameLeftTimeSec() + f); //f초 만큼 추가

		//수리한 발전기 수 증가
		GM->SetDoneMachineNum(GM->GetDoneMachineNum() + 1);
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("SetDoneMachineNum++"));

		//만약 모든 발전기가 수리 완료라면 수리 불가능
		if(GM->GetDoneMachineNum() >= 5 && !IsEnable)
			IsEnable = false;
	}
}

void AMyMachine::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	//들어온 생존자들을 배열에 추가
	surArr.Add(Cast<ASurvivor>(_OtherActor));

	for(int i =0; i< surArr.Num(); i++){
		//들어온 생존자의 수리 가능을 true 로
		surArr[i]->SetIsRepairEnable(true);
		MachineUIVisiblity_Client(true, surArr[i]); //수리 UI 노출
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
	//범위에서 나간 생존자를 수리중 배열에서 지우기
	ASurvivor* surOut = Cast<ASurvivor>(_OtherActor); 

	for (int i = 0; i < surArr.Num(); i++) {
		if(surOut == surArr[i]){
			surArr.RemoveAt(i);
			surOut->SetIsRepairEnable(false); //나간 생존자의 수리 가능을 false 로
			MachineUIVisiblity_Client(false, surOut); //나간 생존자의 기계 수리 UI없애기
			break;
		}
	}
	
}

void AMyMachine::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{

}



