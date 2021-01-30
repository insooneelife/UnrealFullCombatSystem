// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "GamePlay/AbilityEffects/DamageAbilityEffectBase.h"
#include "DamageTrapAbilityEffect.generated.h"

class USceneComponent;
class USoundBase;
class USphereComponent;
class UParticleSystem;
class UPrimitiveComponent;

UCLASS()
class ADamageTrapAbilityEffect : public ADamageAbilityEffectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageTrapAbilityEffect();

protected:
    virtual void BeginPlay() override;

	// Called when the game starts or when spawned
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
    virtual void GetTraceObjects(TArray<FHitResult>& OutHits) const override;

    UFUNCTION(BlueprintCallable)
    void Init(
        float InDamageRadius,
        float InDamage,
        float InImpulse,
        float InTrapRadius,
        float InDuration,
        float InActivationDelay);

    UFUNCTION()
    void OnSphereBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnTargetActivityChanged(EActivity InActivity, bool bInValue);

    void EnableTrap();
    void DisableTrap();

    void SpawnHitParticle();
    void UpdateDecal();
    void PlaySound();

    bool IsImmortal(AActor* InActor) const;

    void ExecuteTrap();

private:

    UPROPERTY(EditAnywhere)
    USphereComponent* SphereComponent;
    
    UPROPERTY(EditAnywhere)
    UDecalComponent* DecalComponent;
    
    UPROPERTY(EditAnywhere)
    UParticleSystem* HitParticle;

    UPROPERTY(EditAnywhere)
    USoundBase* ExplosionSound;

    UPROPERTY(EditAnywhere)
    float TrapRadius;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(EditAnywhere)
    float ActivationDelay;

    UPROPERTY(EditAnywhere)
    FLinearColor Color;

};
