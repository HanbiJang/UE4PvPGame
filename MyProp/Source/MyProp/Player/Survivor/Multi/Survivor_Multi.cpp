// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor_Multi.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

Survivor_Multi::Survivor_Multi()
{
}

Survivor_Multi::~Survivor_Multi()
{
}

void ASurvivor::OnRep_CurrentHealth() {

    OnHealthUpdate();
}

void ASurvivor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	//DOREPLIFETIME(ASurvivor, GetInfo()->fCurHP);
}

void ASurvivor::OnHealthUpdate()
{
    //Client-specific functionality
    if (IsLocallyControlled())
    {
        FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), GetInfo()->fCurHP);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

        if (GetInfo()->fCurHP <= 0)
        {
            FString deathMessage = FString::Printf(TEXT("You have been killed."));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
        }
    }

    //Server-specific functionality
    //if (Role == ROLE_Authority)
    if(HasAuthority())
    {
        FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), GetInfo()->fCurHP);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
    }

    //Functions that occur on all machines. 
    /*
        Any special functionality that should occur as a result of damage or death should be placed here.
    */
}

/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
void ASurvivor::SetCurrentHealth(float healthValue) {

    if (HasAuthority()) {
        //Clamp: 0과 maxHP 값 사이에서 설정함
        GetInfo()->fCurHP = FMath::Clamp(healthValue, 0.f, GetInfo()->fMaxHP);
        OnHealthUpdate();
    }
}

/** Event for taking damage. Overridden from APawn.*/
float ASurvivor::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {

    float damageApplied = GetInfo()->fCurHP - DamageTaken;
    SetCurrentHealth(damageApplied);
    return damageApplied;

}


