// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageSphereAbilityEffect.h"

// Sets default values
ADamageSphereAbilityEffect::ADamageSphereAbilityEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Damage = 15.0f;
    Impulse = 20000.0f;
}

// Called when the game starts or when spawned
void ADamageSphereAbilityEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamageSphereAbilityEffect::GetTraceObjects(TArray<FHitResult>& OutHits) const
{
    FVector Start = GetActorLocation();
    FVector End = GetActorLocation() + FVector(0, 0, 1);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes =
    {
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
    };

    TArray<AActor*> ActorsToIgnore{ GetOwner() };
    UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        Start, End, DamageRadius, ObjectTypes, false, ActorsToIgnore, DebugType, OutHits, true);
}