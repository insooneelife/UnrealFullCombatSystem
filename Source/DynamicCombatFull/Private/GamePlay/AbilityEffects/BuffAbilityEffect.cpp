// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffAbilityEffect.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/StatsManagerComponent.h"

// Sets default values
ABuffAbilityEffect::ABuffAbilityEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Color = FLinearColor(1.0f, 20.0f, 50.0f, 0.0f);
    AttachmentSocket = TEXT("Spine_03");
    TimerInterval = 0.1f;
}

// Called when the game starts or when spawned
void ABuffAbilityEffect::BeginPlay()
{
	Super::BeginPlay();
	
    StatsManagerComponent = Cast<UStatsManagerComponent>(
        GetOwner()->GetComponentByClass(UStatsManagerComponent::StaticClass()));

    AttachToOwner();
    UpdateParticleColor();
    ApplyBuff();
    StartTimer();
}

void ABuffAbilityEffect::ApplyBuff()
{
    if (StatsManagerComponent->IsValidLowLevel())
    {
        StatsManagerComponent->AddModifier(StatType, Value);
    }
}

void ABuffAbilityEffect::AttachToOwner()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (Character != nullptr)
    {
        FAttachmentTransformRules Rules(
            EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);

        AttachToComponent(Character->GetMesh(), Rules, AttachmentSocket);
    }
}

void ABuffAbilityEffect::UpdateParticleColor()
{
    FVector ColorVector = UKismetMathLibrary::Conv_LinearColorToVector(Color);
    ParticleSystemComponent->SetVectorParameter(FName("Color"), ColorVector);
}

void ABuffAbilityEffect::UpdateElapsedTime()
{
    ElapsedTime = ElapsedTime + TimerInterval;

    if (ElapsedTime >= Duration)
    {
        RemoveBuff();
        Deactivate();
    }
}

void ABuffAbilityEffect::StartTimer()
{
    ElapsedTime = 0.0f;
    SetLifeSpan(0.0f);

    UKismetSystemLibrary::K2_SetTimer(this, TEXT("UpdateElapsedTime"), TimerInterval, true);

    ParticleSystemComponent->Activate();
}

void ABuffAbilityEffect::Deactivate()
{
    UKismetSystemLibrary::K2_ClearTimer(this, TEXT("UpdateElapsedTime"));

    ParticleSystemComponent->Deactivate();
    SetLifeSpan(5.0f);
}

void ABuffAbilityEffect::RemoveBuff()
{
    if (StatsManagerComponent->IsValidLowLevel())
    {
        StatsManagerComponent->RemoveModifier(StatType, Value);
        Value = 0.0f;
    }
}

void ABuffAbilityEffect::AdjustBuff(float NewValue, float NewDuration)
{
    float PrevValue = Value;
    Value = NewValue;
    Duration = NewDuration;

    float ValueDifference = Value - PrevValue;

    StartTimer();

    if (ValueDifference > 0.0f)
    {
        if (StatsManagerComponent->IsValidLowLevel())
        {
            StatsManagerComponent->AddModifier(StatType, ValueDifference);
        }
    }
    else if (ValueDifference < 0.0f)
    {
        if (StatsManagerComponent->IsValidLowLevel())
        {
            StatsManagerComponent->RemoveModifier(StatType, ValueDifference);
        }
    }

}