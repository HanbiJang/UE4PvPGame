// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPLAYER_STATE : uint8
{
	IDLE,
	MOVE,
	JUMP,
	DASH,
	HIT, //생존자가 밟아서 스턴, 살인마에게 맞았을 때
	
	//생존자 전용
	MACHINE, //발전기 조작
	DEAD, //기절 상태
	OBJECT, //오브젝트 상태 
	DRAGGED, //살인마에게 끌려가는 상태
	LOCKED, //최면의자에 묶임당하기

	//킬러 전용
	ATTACK,
	RANGEATTACK, //범위 공격
	CATCH, //생존자를 끌고가기
	LOCK, //최면의자에 묶기
};

UENUM(BlueprintType)
enum class EPLAYER_TYPE : uint8
{
	NONE,
	KILLER,
	SURVIVOR,
	RANDOM,
};

class MYPROP_API MyCharacterState
{


public:
	MyCharacterState();
	~MyCharacterState();

};
