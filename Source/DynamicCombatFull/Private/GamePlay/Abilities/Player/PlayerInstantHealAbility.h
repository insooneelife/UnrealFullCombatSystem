// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerInstantHealAbility.generated.h"

class UParticleSystem;
class UExtendedStatComponent;
class UAbilityComponent;
/**
 * 
 */
UCLASS()
class APlayerInstantHealAbility : public APlayerAbilityBase
{
	GENERATED_BODY()
	
public:
    APlayerInstantHealAbility();

public:
    virtual void NativeInit(UAbilityComponent* InAbilityComponent) override;
    virtual void Released() override;
    virtual void Effect() override;
    
private:
    void SpawnParticle();
    void PlaySound();
    void Heal();


private:
    UPROPERTY()
    UExtendedStatComponent* HealthComponent;

    UPROPERTY(EditAnywhere)
    float HealAmount;

    UPROPERTY(EditAnywhere)
    UParticleSystem* HealCastParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* Sound;
};
