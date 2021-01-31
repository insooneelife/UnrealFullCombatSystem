// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "AIInstantHealAbility.generated.h"

class UParticleSystem;
class USoundBase;
/**
 * 
 */
UCLASS()
class AAIInstantHealAbility : public AAIAbilityBase
{
	GENERATED_BODY()
	
public:
    AAIInstantHealAbility();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Released() override;
    virtual void Effect() override;

private:
    void SpawnParticle();
    void PlaySound();
    void Heal();

private:
    UPROPERTY(EditAnywhere)
    UParticleSystem* HealParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* Sound;

    UPROPERTY(EditAnywhere)
    float HealAmount;
};
