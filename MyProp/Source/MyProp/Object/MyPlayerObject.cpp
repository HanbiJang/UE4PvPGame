// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObject.h"
#include "../Player/MyPlayerObjectPawn.h"
#include <math.h>

// Sets default values
AMyPlayerObject::AMyPlayerObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//매시 만들기
	//CreateDefaultSubObject 원하는 컴포넌트를 생성함	
	
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);

	//OnClicked.AddDynamic(this, &AMyPlayerObject::ChangePlayerMesh);

}

// Called when the game starts or when spawned
void AMyPlayerObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float MyGetDistance(FVector a, FVector b) {
	float res = sqrtf(powf(a.X - b.X, 2) + powf(a.Y - b.Y, 2) + powf(a.Z - b.Z, 2));
	return res;
}

//(클릭한 대상) 플레이어, 오브젝트를 폰으로 빙의 시킨다
void AMyPlayerObject::ChangePlayerToObject()
{
	//[플레이어 -> 오브젝트] 월드에서 플레이어 pawn 인 녀석을 가져오기
	ASurvivor* pCharacter = Cast<ASurvivor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (nullptr != pCharacter)
	{	
		if (pCharacter->m_state != EPLAYER_STATE::OBJECT) 
		{
			//거리가 근처일 때만 실행하기
			//GetDistanceTo(this) : 이 오브젝트와 플레이어 캐릭터의 거리
			//MyCharacter 안에 있는 오브젝트 빙의 함수를 실행, 오브젝트의 매시와 스케일 크기를 넘긴다
			if (pCharacter->GetDistanceTo(this) < 400) {
				pCharacter->ChangeToObject(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
			}
		}
		else 
		{ //사물상태 -> 사물상태 
			if (pCharacter->m_PlayerObject->GetStaticMesh() != m_Mesh->GetStaticMesh())
			{
				if (MyGetDistance(pCharacter->m_PlayerObject->GetRelativeLocation(), GetActorLocation()) < 400)
					pCharacter->ChangeObjectMesh(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
			}
		}		
	}
	//else {
	//	//[오브젝트 -> 오브젝트]
	//	AMyPlayerObjectPawn* pPlayerObject = Cast<AMyPlayerObjectPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//	
	//	if (nullptr != pPlayerObject && pPlayerObject->m_ObjectMesh->GetStaticMesh() != m_Mesh->GetStaticMesh())
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("%f"), pPlayerObject->GetDistanceTo(this));
	//		if (MyGetDistance(pPlayerObject->m_ObjectMesh->GetRelativeLocation(), GetActorLocation()) < 400) {
	//			pPlayerObject->ChangeObjectMesh(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
	//		}
	//		
	//	}
	//}
}




