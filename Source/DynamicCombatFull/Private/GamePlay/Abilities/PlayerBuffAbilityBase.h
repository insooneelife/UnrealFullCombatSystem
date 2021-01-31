// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerBuffAbilityBase.generated.h"

class USoundBase;
class UParticleSystem;
class ABuffAbilityEffect;
/**
 * 
 */
UCLASS()
class APlayerBuffAbilityBase : public APlayerAbilityBase
{
	GENERATED_BODY()

public:
    APlayerBuffAbilityBase();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Released() override;
    virtual void Effect() override;
    void SpawnParticle();
    void PlaySound();

    void ApplyBuff();

private:
    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    TSubclassOf<ABuffAbilityEffect> SpawnBuffAbilityEffectClass;

    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    EStat StatType;

    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    float BuffValue;

    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    float BuffDuration;

    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    FLinearColor BuffColor;
    
    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    UParticleSystem* BuffCastParticle;
    
    UPROPERTY(EditAnywhere, Category = "Ability\|Custom")
    USoundBase* Sound;
};
