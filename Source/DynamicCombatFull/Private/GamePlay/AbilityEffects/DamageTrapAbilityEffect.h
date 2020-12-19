// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GamePlay/AbilityEffects/DamageAbilityEffectBase.h"
#include "DamageTrapAbilityEffect.generated.h"

class USoundBase;
class USphereComponent;
class UParticleSystem;

UCLASS()
class ADamageTrapAbilityEffect : public ADamageAbilityEffectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageTrapAbilityEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    virtual void GetTraceObjects(TArray<FHitResult>& OutHits) const override;

    UFUNCTION()
    void OnComponentBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    void EnableTrap();
    void DisableTrap();

    void SpawnHitParticle();
    void UpdateDecal();
    void PlaySound();

    bool IsImmortal(AActor* InActor) const;

private:

    UPROPERTY()
    USphereComponent* SphereComponent;
    
    UPROPERTY()
    UDecalComponent* DecalComponent;
    
    UPROPERTY()
    UParticleSystem* HitParticle;

    UPROPERTY()
        USoundBase* ExplosionSound;

    UPROPERTY(EditAnywhere)
    float TrapRadius;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(EditAnywhere)
    float ActivationDelay;

    FLinearColor Color;

};
