// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "EffectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectAppliedSignature, EEffectType, InType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemovedSignature, EEffectType, InType);

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
    UFUNCTION(BlueprintCallable)
    bool ApplyEffect(EEffectType InType, float InDuration, EApplyEffectMethod InMethod, AActor* InApplier);

    UFUNCTION(BlueprintCallable)
    bool IsEffectApplied(EEffectType InType) const;

    UFUNCTION(BlueprintCallable)
    AActor* GetEffectApplier(EEffectType InType) const;

    UFUNCTION(BlueprintCallable)
    float GetBackstabDamage() const { return BackstabDamage; }

public:
    bool ApplyBackstabEffect(float InDuration, EApplyEffectMethod InMethod, AActor* InApplier, float InDamage);
    bool ApplyBurningEffect(float InDuration, EApplyEffectMethod InMethod, AActor* InApplier, float InDamage);
    void AdjustEffectTime(EEffectType InType, float InNewDuration);

    bool IsAnyEffectApplied(TArray<EEffectType> InTypes) const;
    void PrintEffects() const;

private:
    void RemoveEffect(EEffectType InType);
    void UpdateEffectsDuration();
    void RemoveEffects(TArray<EEffectType> InTypes);
    void UpdateEffect(EEffectType InType, float InDuration, EApplyEffectMethod InMethod, AActor* InApplier);
    FEffect GetEffect(EEffectType InType) const;
    int GetEffectIndex(EEffectType InType) const;

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

    TArray<FEffect> AppliedEffects;

    float UpdateInterval;
};