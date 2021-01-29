// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectsComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/CanGetEffects.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UObject/Class.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UEffectsComponent::UEffectsComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    UpdateInterval = 0.1f;
}


// Called when the game starts
void UEffectsComponent::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &UEffectsComponent::UpdateEffectsDuration, UpdateInterval, true);
}


bool UEffectsComponent::ApplyEffect(
    EEffectType InType, float InDuration, EApplyEffectMethod InMethod, AActor* const InApplier)
{
    TWeakObjectPtr<AActor> OwnerPtr(GetOwner());
    ICanGetEffects* IEffect = Cast<ICanGetEffects>(OwnerPtr.Get());

    if (IEffect != nullptr)
    {
        if (!IEffect->CanEffectBeApplied(InType, InApplier))
        {
            return false;
        }
    }

    UpdateEffect(InType, InDuration, InMethod, InApplier);
    OnEffectApplied.Broadcast(InType);
    return true;

}

bool UEffectsComponent::IsEffectApplied(EEffectType InType) const
{
    return GetEffectIndex(InType) >= 0;
}

AActor* UEffectsComponent::GetEffectApplier(EEffectType InType) const
{
    return GetEffect(InType).Applier;
}

bool UEffectsComponent::ApplyBackstabEffect(
    float InDuration, EApplyEffectMethod InMethod, AActor* const InApplier, float InDamage)
{
    TWeakObjectPtr<AActor> OwnerPtr(GetOwner());
    ICanGetEffects* IEffect = Cast<ICanGetEffects>(OwnerPtr.Get());

    if (IEffect != nullptr)
    {
        if (!IEffect->CanEffectBeApplied(EEffectType::Backstab, InApplier))
        {
            return false;
        }
    }

    BackstabDamage = InDamage;
    UpdateEffect(EEffectType::Backstab, InDuration, InMethod, InApplier);
    OnEffectApplied.Broadcast(EEffectType::Backstab);
    return true;

}

bool UEffectsComponent::ApplyBurningEffect(
    float InDuration, EApplyEffectMethod InMethod, AActor* const InApplier, float InDamage)
{
    TWeakObjectPtr<AActor> OwnerPtr(GetOwner());
    ICanGetEffects* IEffect = Cast<ICanGetEffects>(OwnerPtr.Get());

    if (IEffect != nullptr)
    {
        if (!IEffect->CanEffectBeApplied(EEffectType::Burning, InApplier))
        {
            return false;
        }
    }

    BurningDamage = InDamage;
    UpdateEffect(EEffectType::Burning, InDuration, InMethod, InApplier);
    OnEffectApplied.Broadcast(EEffectType::Burning);
    return true;
}

void UEffectsComponent::AdjustEffectTime(EEffectType InType, float InNewDuration)
{
    int LIndex = GetEffectIndex(InType);

    if (LIndex >= 0)
    {
        AppliedEffects[LIndex].Duration = InNewDuration;
    }
}

bool UEffectsComponent::IsAnyEffectApplied(TArray<EEffectType> InTypes) const
{
    for (EEffectType EType : InTypes)
    {
        if (IsEffectApplied(EType))
        {
            return true;
        }
    }

    return false;
}

void UEffectsComponent::PrintEffects() const
{
    TWeakObjectPtr<AActor> OwnerPtr(GetOwner());
    if (OwnerPtr.IsValid())
    {
        FString DisplayName = UKismetSystemLibrary::GetDisplayName(OwnerPtr.Get());
        GameUtils::PrintEffects(GetWorld(), DisplayName, AppliedEffects);
    }
}


void UEffectsComponent::RemoveEffect(EEffectType InType)
{
    int Index = AppliedEffects.FindLastByPredicate([InType](const FEffect& InEffect) { return InEffect.Type == InType; });

    if (Index != INDEX_NONE)
    {
        AppliedEffects.RemoveAt(Index);
        OnEffectRemoved.Broadcast(InType);
    }
}

void UEffectsComponent::UpdateEffectsDuration()
{
    if (AppliedEffects.Num() > 0)
    {
        TArray<FEffect> LEffectsToRemove;

        for (int i = 0; i < AppliedEffects.Num(); ++i)
        {
            AppliedEffects[i].Duration = AppliedEffects[i].Duration - UpdateInterval;

            if (AppliedEffects[i].Duration <= UpdateInterval)
            {
                LEffectsToRemove.Add(AppliedEffects[i]);
            }
        }

        for (const FEffect& Effect : LEffectsToRemove)
        {
            RemoveEffect(Effect.Type);
        }

    }
}

void UEffectsComponent::RemoveEffects(TArray<EEffectType> InTypes)
{
    for (EEffectType Type : InTypes)
    {
        RemoveEffect(Type);
    }
}


void UEffectsComponent::UpdateEffect(
    EEffectType InType, float InDuration, EApplyEffectMethod InMethod, AActor* const InApplier)
{
    int LIndex = GetEffectIndex(InType);

    if (LIndex >= 0)
    {
        if (InMethod == EApplyEffectMethod::Stack)
        {
            float NewDuration = AppliedEffects[LIndex].Duration + InDuration;
            AppliedEffects[LIndex].Duration = NewDuration;
        }
        else
        {
            AppliedEffects[LIndex].Duration = InDuration;
            AppliedEffects[LIndex].Applier = InApplier;
        }
    }
    else
    {
        FEffect Effect;
        Effect.Type = InType;
        Effect.Duration = InDuration;
        Effect.Applier = InApplier;

        AppliedEffects.Add(Effect);
    }
}




FEffect UEffectsComponent::GetEffect(EEffectType InType) const
{
    for (const FEffect& Effect : AppliedEffects)
    {
        if (Effect.Type == InType)
        {
            return Effect;
        }
    }

    return FEffect();
}

int UEffectsComponent::GetEffectIndex(EEffectType InType) const
{
    for (int i = 0; i < AppliedEffects.Num(); ++i)
    {
        if (AppliedEffects[i].Type == InType)
        {
            return i;
        }
    }
    return -1;
}
