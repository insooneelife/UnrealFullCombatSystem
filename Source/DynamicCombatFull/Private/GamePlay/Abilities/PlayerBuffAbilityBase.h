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

    virtual void Released() override;
    virtual void Effect() override;
    void SpawnParticle();
    void PlaySound();

    void ApplyBuff();

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<ABuffAbilityEffect> SpawnBuffAbilityEffectClass;

    EStat StatType;
    float BuffValue;
    float BuffDuration;
    FLinearColor BuffColor;
    
    UPROPERTY()
    UParticleSystem* BuffCastParticle;
    
    UPROPERTY()
    USoundBase* Sound;
};
