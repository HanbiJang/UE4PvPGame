// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "../MyCharacter.h"
#include "../Killer/Killer.h"


UMyAnimInstance::UMyAnimInstance() {

}

void UMyAnimInstance::NativeInitializeAnimation()
{

}

void UMyAnimInstance::NativeBeginPlay()
{
}

void UMyAnimInstance::NativeUpdateAnimation(float _fDT)
{

	// 게임 시작 후 매프레임마다 호출됨
	AMyCharacter* pCharacter = Cast<AMyCharacter>(TryGetPawnOwner());

	if (nullptr != pCharacter)
	{
		UCharacterMovementComponent* pMovementComponent = pCharacter->GetCharacterMovement();
		m_eState = pCharacter->GetState(); //상태
		m_bGround = pCharacter->GetMovementComponent()->IsMovingOnGround(); //땅에 있는지 여부
	}
}

void UMyAnimInstance::AnimNotify_Idle()
{
	AMyCharacter* pCharacter = Cast<AMyCharacter>(TryGetPawnOwner());

	if (nullptr != pCharacter) {
		pCharacter->ChangeState(EPLAYER_STATE::IDLE);
	}
}

void UMyAnimInstance::AnimNotify_JumpAction()
{
	// 게임 시작 후 매프레임마다 호출됨
	AMyCharacter* pCharacter = Cast<AMyCharacter>(TryGetPawnOwner());

	if (nullptr != pCharacter)
	{
		pCharacter->JumpAction();
	}
}

void UMyAnimInstance::AnimNotify_AttackAction() {
	AKiller* pKiller = Cast<AKiller>(TryGetPawnOwner());

	if (nullptr != pKiller) {
		pKiller->AttackAction();
	}
}