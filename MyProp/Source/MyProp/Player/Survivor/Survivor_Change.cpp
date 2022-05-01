// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor_Change.h"

//인간에서 사물로 변신
void ASurvivor::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	isObject = true;
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
	m_PlayerObject->SetRelativeScale3D(fscale);
	m_PlayerObject->SetStaticMesh(mesh);

	//시점 옮기기...
	//카메라만 옮기기
	m_Arm->DetachFromParent(true);
	m_Arm->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);

	//사운드 컴포넌트 이동
	AC_HeartBeat->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);
	AC_Chase->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);

	//0.1초 뒤에 오브젝트 물리 켜기
	GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &ASurvivor::SetSimulatePhysicsTrue, 0.1f, false);

	//변신 가능 상태 조절 1.5초 뒤에 변신 가능해지기
	GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);

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
		isObject = false;
		bChangeEnable = false; //과다변신 막기

		//시점 변경
		m_Arm->DetachFromParent(true);
		m_Arm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		m_Arm->SetRelativeLocation(FVector(0, 0, 0));

		//사운드 컴포넌트 이동
		AC_HeartBeat->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		AC_Chase->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector originalPos = m_PlayerObject->GetRelativeLocation(); //인간폼 돌아가기 당시 원래 [메시의] 위치 저장

		m_PlayerObject->SetSimulatePhysics(false); //물리 끄기

		//매시 설정
		m_PlayerObject->SetAllPhysicsRotation(FRChange); //각도
		m_PlayerObject->SetRelativeLocation(FVChange  //위치
			+ FVector(0, 0, m_PlayerObject->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//인간폼의 위치를 오브젝트가 있던 액터 위치로 설정하기
		SetActorLocation(originalPos);

		//1.5초 뒤에 변신 가능해지기
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);

		//상태 변화
		ChangeState(EPLAYER_STATE::IDLE);
	}
}