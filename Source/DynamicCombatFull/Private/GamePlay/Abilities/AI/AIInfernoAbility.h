// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "AIInfernoAbility.generated.h"

class UParticleSystem;
class USoundBase;
/**
 * 
 */
UCLASS()
class AAIInfernoAbility : public AAIAbilityBase
{
	GENERATED_BODY()
	
public:
    AAIInfernoAbility();

    virtual void Released() override;
    virtual void Effect() override;

private:
    void SpawnParticle();
    void PlaySound();

private:
    UPROPERTY(EditAnywhere)
    UParticleSystem* HitParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* Sound;

    UPROPERTY(EditAnywhere)
    float Radius;

    UPROPERTY(EditAnywhere)
    float Range;
};
