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

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void NativeInit(UAbilityComponent* InAbilityComponent) override;
    virtual void Released() override;
    virtual void Effect() override;
    
private:
    void SpawnParticle();
    void PlaySound();
    void Heal();


private:
    TWeakObjectPtr<UExtendedStatComponent> HealthComponent;

    UPROPERTY(EditAnywhere)
    UParticleSystem* HealCastParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* Sound;

    UPROPERTY(EditAnywhere)
    float HealAmount;
};
