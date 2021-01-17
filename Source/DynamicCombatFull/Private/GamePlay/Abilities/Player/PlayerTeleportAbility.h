// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerTeleportAbility.generated.h"

class UParticleSystem;
class USoundBase;
/**
 * 
 */
UCLASS()
class APlayerTeleportAbility : public APlayerAbilityBase
{
	GENERATED_BODY()

public:
    APlayerTeleportAbility();

public:
    virtual void Tick(float DeltaTime) override;
    virtual void Released() override;
    virtual void Pressed() override;
    virtual void Effect() override;

private:
    void UpdateEffect();
    void SpawnTeleportParticle();
    void PlaySound();
    bool AbilityTeleport();

private:
    UPROPERTY(EditAnywhere)
    float Range;

    UPROPERTY(EditAnywhere)
    float MaxSurfaceAngle;

    UPROPERTY(EditAnywhere)
    UParticleSystem* TeleportParticle;
    
    UPROPERTY(EditAnywhere)
    USoundBase* Sound;
};
