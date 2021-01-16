// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerInfernoAbility.generated.h"

class UParticleSystem;
class USoundBase;
class ADamageSphereAbilityEffect;
/**
 * 
 */
UCLASS()
class APlayerInfernoAbility : public APlayerAbilityBase
{
	GENERATED_BODY()
	
public:
    APlayerInfernoAbility();

public:
    virtual void Tick(float DeltaTime) override;
    virtual void Released() override;
    virtual void Effect() override;
    virtual void Pressed() override;

private:
    void UpdateEffect();
    void SpawnParticle();
    void PlaySound();

private:
    UPROPERTY(EditAnywhere)
    float Range;

    UPROPERTY(EditAnywhere)
    float Radius;

    UPROPERTY(EditAnywhere)
    float MaxSurfaceAngle;

    UPROPERTY(EditAnywhere)
    UParticleSystem* HitParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* Sound;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADamageSphereAbilityEffect> DamageSphereEffectClass;
};
