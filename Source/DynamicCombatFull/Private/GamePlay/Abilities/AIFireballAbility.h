// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "AIFireballAbility.generated.h"

class AFireballProjectileAbilityEffect;
/**
 * 
 */
UCLASS()
class AAIFireballAbility : public AAIAbilityBase
{
	GENERATED_BODY()
public:
    AAIFireballAbility();

public:
    virtual void Released() override;
    virtual void Effect() override;
    virtual void Ended(EAbilityEndResult Result) override;

protected:
    void UpdateEffect();
    void ResetMontageCounter();
    void UpdateMontageCounter();
    void UpdateSocket();
    void StartTimer();
    void StopTimer();

    void Delayed_ResetMontageCounter();

private:

    UPROPERTY(EditAnywhere)
    TSubclassOf<AFireballProjectileAbilityEffect> FireballProjectileClass;

    int MontageCounter;
    bool bUseLeftHandSocket;
    FTimerHandle UpdateEffectTimerHandle;
    FTimerHandle RetriggerableTimerHandle;
};
