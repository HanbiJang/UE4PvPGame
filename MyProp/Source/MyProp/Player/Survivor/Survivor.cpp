// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
#include "../MyPlayerObjectPawn.h"

ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	FRChange(0, 0, 0)
{
	//======플레이어 (초기) 데이터 설정======
	const FSurvivorInfo* Info = nullptr;

	//데이터 테이블 블루프린트에서 가져오기
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable
	(TEXT("DataTable'/Game/Blueprints/DT_Survivor.DT_Survivor'"));
	
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;

		if (m_PlayerTable) {
			Info = m_PlayerTable->FindRow<FSurvivorInfo>(FName(TEXT("Survivor1")), TEXT(""));
			SetInfo(*Info);
		}
	}

}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);
}

void ASurvivor::Interaction()
{
	//발전기, 문 등과 상호작용
}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();

	//======블레이어 변신용 사물 오브젝트 블루프린트======
	//가져와서 스폰시키기
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

	// 현재 체력을 이전프레임 체력으로 갱신
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;

}

void ASurvivor::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//플레이어 상태 변경
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //변신 당시 원래 위치 저장

	//원래 [인간형] 오브젝트 다른 곳으로 치우기 && 각도 시작과 같이 설정하기
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//물리 끄기
	m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(false);

	//물체 위치를 인간폼이 있던 곳으로 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));

	//클릭한 오브젝트 메시의 크기와 같게 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetRelativeScale3D(fscale);
	//매시 설정
	m_PlayerObjectPawn->m_ObjectMesh->SetStaticMesh(mesh);

	//시점 옮기기...
	GetWorld()->GetFirstPlayerController()->Possess(m_PlayerObjectPawn);

	//0.1초 뒤에 오브젝트 물리 켜기
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FPhysicsTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetSimulatePhysicsTrue, 0.1f, false);

	//변신 가능 상태 조절
	//1.5초 뒤에 변신 가능해지기
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FChangeEnableTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);

	//캐릭터 정보 옮기기
	m_PlayerObjectPawn->SetInfo(m_Info);

}


