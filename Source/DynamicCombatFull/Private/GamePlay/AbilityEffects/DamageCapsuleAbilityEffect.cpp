// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCapsuleAbilityEffect.h"

// Sets default values
ADamageCapsuleAbilityEffect::ADamageCapsuleAbilityEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Damage = 15.0f;
    Impulse = 20000.0f;
}

// Called when the game starts or when spawned
void ADamageCapsuleAbilityEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ADamageCapsuleAbilityEffect::GetTraceObjects(TArray<FHitResult>& OutHits)
{
    FVector Start = GetActorLocation();
    FVector End = GetActorLocation() + FVector(0, 0, 1);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes =
    {
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
    };

    TArray<AActor*> ActorsToIgnore{ GetOwner() };
    UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(),
        Start, End, DamageRadius, HalfHeight, ObjectTypes, false, ActorsToIgnore, DebugType, OutHits, true);
}
