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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
    UFUNCTION(BlueprintCallable)
    void Init(float InDamage, float InInitialSpeed, float InLifeTime, AActor* InHomingTarget, bool bInApplyStun);

    void EnableHomingProjectile();
    void DisableHomingProjectile();

    void ApplyHitImpulse(UPrimitiveComponent* Component, FVector HitNormal, FName BoneName);

    UFUNCTION()
    void OnHit(const FHitResult& Hit);
    bool IsEnemy(AActor* Target) const;
    void UpdateHomingProjectile();

public:
    float GetDamage() const { return Damage; }
    float GetInitialSpeed() const { return InitialSpeed; }
    float GetLifeTime() const { return LifeTime; }
    AActor* GetHomingTarget() const { return HomingTarget.Get(); }
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

    TWeakObjectPtr<AActor> HomingTarget;

    UPROPERTY(EditAnywhere)
    float Damage;

    UPROPERTY(EditAnywhere)
    float InitialSpeed;

    UPROPERTY(EditAnywhere)
    float LifeTime;

    UPROPERTY(EditAnywhere)
    bool bApplyStun;

    UPROPERTY(EditAnywhere)
    float HeadShotDamageMultiplier;

    UPROPERTY(EditAnywhere)
    float HomingInterpSpeed;

    UPROPERTY(EditAnywhere)
    float StopHomingDistance;

    UPROPERTY(EditAnywhere)
    float ImpulsePower;

    UPROPERTY(EditAnywhere)
    UParticleSystem* HitParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* CollisionSphere;

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* ParticleSystem;

    UPROPERTY(EditAnywhere)
    UCollisionHandlerComponent* CollisionHandler;

    UPROPERTY(EditAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    FTimerHandle UpdateHomingProjectileTimerHandle;

};
