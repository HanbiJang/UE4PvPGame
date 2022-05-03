// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "MyInfo.generated.h" //staticStruct 오류 해결

/**
 * 
 */
class MYPROP_API MyInfo
{
public:
	MyInfo();
	~MyInfo();
};


//생존자
USTRUCT(Atomic, BlueprintType)
struct FSurvivorInfo : public FTableRowBase // 데이터 테이블과 연동되는 구조체
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FString strClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fMaxHP; //[생존자] 최대 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fCurHP; //[생존자] 현재 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iMachine; //[생존자] 발전기 돌린 수 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fMaxSP; //[생존자] 최대 스테미나

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fCurSP; //[생존자] 스테미나

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iStun; //[생존자] 살인마 스턴 수 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iCoin; //[공통] 코인 수 


};

//살인마
USTRUCT(Atomic, BlueprintType)
struct FKillerInfo : public FTableRowBase // 데이터 테이블과 연동되는 구조체
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FString strClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fMaxSP; //[공통] 최대 스테미나

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fCurSP; //[공통] 스테미나

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iCoin; //[공통] 코인 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iKill; //[살인마] 킬 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fAllHit; //[살인마] 총 입힌 피해

};

//킬러 공격 이펙트
UENUM(BlueprintType)
enum class EKillerEffect : uint8
{
	ATTACK,
	Q,
	E,
	RIGHTCLICK,
	PARTICLEHIT,

};