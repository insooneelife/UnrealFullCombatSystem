// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectsComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/CanGetEffects.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UObject/Class.h"

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

    // ...

}


bool UEffectsComponent::ApplyEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier)
{
    ICanGetEffects* IEffect = Cast<ICanGetEffects>(GetOwner());

    if (IEffect != nullptr)
    {
        if (!IEffect->CanEffectBeApplied(Type, Applier))
        {
            return false;
        }
    }

    UpdateEffect(Type, Duration, Method, Applier);
    OnEffectApplied.Broadcast(Type);
    return true;

}

void UEffectsComponent::RemoveEffect(EEffectType Type)
{
    int Index = AppliedEffects.FindLastByPredicate([Type](const FEffect& Effect) { return Effect.Type == Type; });

    if (Index != INDEX_NONE)
    {
        AppliedEffects.RemoveAt(Index);
        OnEffectRemoved.Broadcast(Type);
    }
}

void UEffectsComponent::AdjustEffectTime(EEffectType Type, float NewDuration)
{
    int LIndex = GetEffectIndex(Type);

    if (LIndex >= 0)
    {
        AppliedEffects[LIndex].Duration = NewDuration;
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

void UEffectsComponent::RemoveEffects(TArray<EEffectType> Types)
{
    for (EEffectType Type : Types)
    {
        RemoveEffect(Type);
    }
}

bool UEffectsComponent::ApplyBackstabEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage)
{
    ICanGetEffects* IEffect = Cast<ICanGetEffects>(GetOwner());

    if (IEffect != nullptr)
    {
        if (!IEffect->CanEffectBeApplied(EEffectType::Backstab, Applier))
        {
            return false;
        }
    }

    BackstabDamage = Damage;
    UpdateEffect(EEffectType::Backstab, Duration, Method, Applier);
    OnEffectApplied.Broadcast(EEffectType::Backstab);
    return true;

}

bool UEffectsComponent::ApplyBurningEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage)
{
    ICanGetEffects* IEffect = Cast<ICanGetEffects>(GetOwner());

    if (IEffect != nullptr)
    {
        if (!IEffect->CanEffectBeApplied(EEffectType::Burning, Applier))
        {
            return false;
        }
    }

    BurningDamage = Damage;
    UpdateEffect(EEffectType::Burning, Duration, Method, Applier);
    OnEffectApplied.Broadcast(EEffectType::Burning);
    return true;
}

void UEffectsComponent::UpdateEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier)
{
    int LIndex = GetEffectIndex(Type);

    if (LIndex >= 0)
    {
        if (Method == EApplyEffectMethod::Stack)
        {
            float NewDuration = AppliedEffects[LIndex].Duration + Duration;
            AppliedEffects[LIndex].Duration = NewDuration;
        }
        else
        {
            AppliedEffects[LIndex].Duration = Duration;
            AppliedEffects[LIndex].Applier = Applier;
        }
    }
    else
    {
        FEffect Effect;
        Effect.Type = Type;
        Effect.Duration = Duration;
        Effect.Applier = Applier;

        AppliedEffects.Add(Effect);
    }
}

bool UEffectsComponent::IsEffectApplied(EEffectType Type) const
{
    return GetEffectIndex(Type) >= 0;
}

bool UEffectsComponent::IsAnyEffectApplied(TArray<EEffectType> Types) const
{
    for (EEffectType EType : Types)
    {
        if (IsEffectApplied(EType))
        {
            return true;
        }
    }

    return false;
}

FEffect UEffectsComponent::GetEffect(EEffectType Type) const
{
    for (const FEffect& Effect : AppliedEffects)
    {
        if (Effect.Type == Type)
        {
            return Effect;
        }
    }

    return FEffect();
}

void UEffectsComponent::PrintEffects()
{
    FString DisplayName = UKismetSystemLibrary::GetDisplayName(GetOwner());

    UKismetSystemLibrary::PrintString(GetWorld(), DisplayName);

    FString LEffects;
    for (const FEffect& Effect : AppliedEffects)
    {
        FString A = GetEEffectTypeAsString(Effect.Type);
        FString B = UKismetStringLibrary::Conv_FloatToString(Effect.Duration);

        LEffects.Append(A);
        LEffects.Append(TEXT(":"));
        LEffects.Append(B);
        LEffects.Append(TEXT("***"));
    }


    UKismetSystemLibrary::PrintString(
        GetWorld(), LEffects, true, true,
        FLinearColor::Red, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));
}

int UEffectsComponent::GetEffectIndex(EEffectType Type) const
{
    for (int i = 0; i < AppliedEffects.Num(); ++i)
    {
        if (AppliedEffects[i].Type == Type)
        {
            return i;
        }
    }
    return -1;
}

AActor* UEffectsComponent::GetEffectApplier(EEffectType Type) const
{
    return GetEffect(Type).Applier;
}


FString UEffectsComponent::GetEEffectTypeAsString(EEffectType EnumValue)
{
    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEffectType"), true);
    if (!EnumPtr) return FString("Invalid");
    return EnumPtr->GetNameByValue((int64)EnumValue).ToString();
}