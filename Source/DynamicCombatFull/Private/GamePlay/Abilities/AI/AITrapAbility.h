// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "AITrapAbility.generated.h"

/**
 * 
 */
UCLASS()
class AAITrapAbility : public AAIAbilityBase
{
	GENERATED_BODY()
	
public:
    AAITrapAbility();

    virtual void Released() override;
    virtual void Effect() override;

private:
    void PlaySound();

private:
    UPROPERTY(EditAnywhere)
    float TrapRadius;

    UPROPERTY(EditAnywhere)
    float DamageRadius;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(EditAnywhere)
    float ActivationDelay;

    UPROPERTY(EditAnywhere)
    float Range;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    USoundBase* Sound;
};
