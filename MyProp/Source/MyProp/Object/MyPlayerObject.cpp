// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObject.h"
//#include "../Player/MyPlayerObjectPawn.h"
#include "../Player/MyCharacterObject.h"

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

void AMyPlayerObject::ChangePlayerToObject()
{
	//(클릭한 대상) 플레이어, 오브젝트를 폰으로 빙의 시킨다

	//[플레이어 -> 오브젝트] 월드에서 플레이어 pawn 인 녀석을 가져오기
	AMyCharacter* pCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (nullptr != pCharacter)
	{		
		//MyCharacter 안에 있는 오브젝트 빙의 함수를 실행, 오브젝트의 매시와 스케일 크기를 넘긴다
		pCharacter->ChangeToObject(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
	}
	else {
		//[오브젝트 -> 플레이어]
		//UE_LOG(LogTemp, Log, TEXT("null!"));

		//AMyPlayerObjectPawn* pPlayerObject = Cast<AMyPlayerObjectPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		AMyCharacterObject* pPlayerObject = Cast<AMyCharacterObject>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		
		if (nullptr != pPlayerObject && pPlayerObject->m_ObjectMesh->GetStaticMesh() != m_Mesh->GetStaticMesh()) {
			pPlayerObject->ChangeObjectMesh(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
		}
	}

}



