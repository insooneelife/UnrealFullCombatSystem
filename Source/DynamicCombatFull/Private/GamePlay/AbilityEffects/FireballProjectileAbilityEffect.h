// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireballProjectileAbilityEffect.generated.h"

class UParticleSystem;
class USoundBase;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UCollisionHandlerComponent;

UCLASS()
class AFireballProjectileAbilityEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireballProjectileAbilityEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    void EnableHomingProjectile();
    void DisableHomingProjectile();

    void ApplyHitImpulse(UPrimitiveComponent* Component, FVector HitNormal, FName BoneName);
    void OnHit(const FHitResult& Hit);
    bool IsEnemy(AActor* Target);
    void UpdateHomingProjectile();

public:
    float GetDamage() const { return Damage; }
    float GetInitialSpeed() const { return InitialSpeed; }
    float GetLifeTime() const { return LifeTime; }
    AActor* GetHomingTarget() const { return HomingTarget; }
    bool ShouldApplyStun() const { return bApplyStun; }

    float GetHeadShotDamageMultiplier() const { return HeadShotDamageMultiplier; }
    float GetHomingInterpSpeed() const { return HomingInterpSpeed; }
    float GetStopHomingDistance() const { return StopHomingDistance; }
    float GetImpulsePower() const { return ImpulsePower; }

    void SetDamage(float InDamage) { Damage = InDamage; }
    void SetInitialSpeed(float InInitialSpeed) { InitialSpeed = InInitialSpeed; }
    void SetLifeTime(float InLifeTime) { LifeTime = InLifeTime; }
    void SetHomingTarget(AActor* InHomingTarget) { HomingTarget = InHomingTarget; }
    void SetApplyStun(bool bInApplyStun) { bApplyStun = bInApplyStun; }

private:

    UPROPERTY(EditAnywhere)
    float Damage;

    UPROPERTY(EditAnywhere)
    float InitialSpeed;

    UPROPERTY(EditAnywhere)
    float LifeTime;

    UPROPERTY(EditAnywhere)
    AActor* HomingTarget;

    UPROPERTY(EditAnywhere)
    bool bApplyStun;

    float HeadShotDamageMultiplier;
    float HomingInterpSpeed;
    float StopHomingDistance;
    float ImpulsePower;

    UPROPERTY()
    UParticleSystem* HitParticle;

    UPROPERTY()
    UParticleSystem* FireballHitParticle;

    UPROPERTY()
    USoundBase* HitSound;

    UPROPERTY()
    UStaticMeshComponent* CollisionSphere;

    UPROPERTY()
    UParticleSystemComponent* ParticleSystem;

    UPROPERTY()
    UCollisionHandlerComponent* CollisionHandler;

    UPROPERTY()
    UProjectileMovementComponent* ProjectileMovement;

};
