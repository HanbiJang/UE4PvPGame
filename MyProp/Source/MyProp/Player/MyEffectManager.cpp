// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffectManager.h"
#include <MyProp/Player/MyEffect.h> //static 변수 사용 관련 1

//static 변수 사용 관련 2
UMyEffectManager* UMyEffectManager::m_This = nullptr;
UWorld* UMyEffectManager::m_World = nullptr;

UMyEffectManager::UMyEffectManager()
{
}

UMyEffectManager* UMyEffectManager::GetInst(UWorld* _world)
{
	if (nullptr == m_This)
	{
		// 만들어서 준다.
		m_This = NewObject<UMyEffectManager>();
		m_This->AddToRoot(); // G.C 참조 계산에서 가창 최상위에 위치한다. ==> GC 에 의해서 삭제를 방지

		m_World = _world;
	}

	// 변경된 월드에서 객체를 요청한 경우
	if (m_World != _world)
	{
		m_World = _world;
	}

	return m_This;
}

void UMyEffectManager::Destroy()
{
	if (nullptr != m_This)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}


void UMyEffectManager::CreateEffect(EKillerEffect _type, FTransform _trans, ULevel* _Level)
{
	FActorSpawnParameters SpawnParam = {};

	SpawnParam.OverrideLevel = _Level;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	AMyEffect* pEffect = m_World->SpawnActor<AMyEffect>(AMyEffect::StaticClass(), _trans, SpawnParam);
	if (pEffect != nullptr) {
	pEffect->SetEffectType(_type);
	pEffect->FinishSpawning(pEffect->GetTransform());
	}
}