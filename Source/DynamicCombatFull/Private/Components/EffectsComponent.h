// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "EffectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectAppliedSignature, EEffectType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemovedSignature, EEffectType, Type);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEffectsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UEffectsComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    bool ApplyEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier);

    void RemoveEffect(EEffectType Type);

    void AdjustEffectTime(EEffectType Type, float NewDuration);

    void UpdateEffectsDuration();

    void RemoveEffects(TArray<EEffectType> Types);

    bool ApplyBackstabEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage);

    bool ApplyBurningEffect(float Duration, EApplyEffectMethod Method, AActor* Applier, float Damage);

    void UpdateEffect(EEffectType Type, float Duration, EApplyEffectMethod Method, AActor* Applier);

    bool IsEffectApplied(EEffectType Type) const;

    bool IsAnyEffectApplied(TArray<EEffectType> Types) const;

    FEffect GetEffect(EEffectType Type) const;

    void PrintEffects();

    int GetEffectIndex(EEffectType Type) const;

    AActor* GetEffectApplier(EEffectType Type) const;

    static FString GetEEffectTypeAsString(EEffectType EnumValue);


public:
    UPROPERTY(BlueprintAssignable)
        FEffectAppliedSignature OnEffectApplied;

    UPROPERTY(BlueprintAssignable)
        FOnEffectRemovedSignature OnEffectRemoved;

private:
    UPROPERTY(EditAnywhere)
        float BackstabDamage;

    UPROPERTY(EditAnywhere)
        float BurningDamage;

    UPROPERTY(EditAnywhere)
        TArray<FEffect> AppliedEffects;

    UPROPERTY(EditAnywhere)
        float UpdateInterval;

};