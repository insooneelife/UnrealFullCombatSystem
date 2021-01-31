// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerVortexAbility.generated.h"

class UParticleSystem;
class USoundBase;
/**
 * 
 */
UCLASS()
class APlayerVortexAbility : public APlayerAbilityBase
{
	GENERATED_BODY()
	
public:
    APlayerVortexAbility();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void Released() override;
    virtual void Pressed() override;
    virtual void Effect() override;
    virtual void Ended(EAbilityEndResult Result) override;

    void AttempToCastEvent();
    void ClearAttemptToCastTimer();
    void SpawnParticle();
    void DeactivateParticle();
    void PlaySound();
    void StopSound();
    void UpdateVortex();

private:
    void AttackToEnemies();
    bool AttemptToCast();

private:
    UPROPERTY()
    UParticleSystemComponent* ParticleComponent;

    UPROPERTY()
    UAudioComponent* SoundComponent;

    UPROPERTY(EditAnywhere)
    float Range;

    UPROPERTY(EditAnywhere)
    float Radius;

    UPROPERTY(EditAnywhere)
    float MaxSurfaceAngle;

    UPROPERTY(EditAnywhere)
    float VortexSpeed;

    UPROPERTY(EditAnywhere)
    float CollisionHeight;

    UPROPERTY(EditAnywhere)
    float FirstUseManaCost;

    UPROPERTY(EditAnywhere)
    UParticleSystem* VortexParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* Sound;

    bool bEffectEnabled;
    FVector VortexLocation;

    FTimerHandle AttempToCastTimerHandle;
};
