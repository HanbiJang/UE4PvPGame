// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObject.h"

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

void AMyPlayerObject::ChangePlayerMesh()
{
	//(클릭한 대상) 플레이어의 매시를 변경시킨다

	//월드에서 플레이어 pawn 인 녀석을 가져오기
	AMyCharacter* pCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (nullptr != pCharacter)
	{		
		//MyCharacter 안에 있는 매시 변경 함수를 실행, 오브젝트의 매시와 스케일 크기를 넘긴다
		pCharacter->ChangeMesh(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("null!"));
	}
}

//void AMyPlayerObject::ChangePlayerMesh(AActor* target, FKey ButtomPressed) //AddDynamic으로 추가해주기 위해서 인수를 맞춰주었음
//{
//	UE_LOG(LogTemp, Log, TEXT("Clicked!"))
//}


