// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Projectiles/ArrowProjectileBase.h"
#include "ExplosiveArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AExplosiveArrowProjectile : public AArrowProjectileBase
{
	GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AExplosiveArrowProjectile();

protected:
    virtual void OnArrowHit(const FHitResult& InHit) override;


private:
    UPROPERTY(EditAnywhere, Category = Projectile)
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    UParticleSystem* ExplosionParticleObject;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    USoundBase* ExplosionSoundObject;
};
