// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageAbilityEffectBase.h"
#include "DamageCapsuleAbilityEffect.generated.h"

UCLASS()
class ADamageCapsuleAbilityEffect : public ADamageAbilityEffectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageCapsuleAbilityEffect();

protected:

public:	
    virtual void GetTraceObjects(TArray<FHitResult>& OutHits) const override;

    UFUNCTION(BlueprintCallable)
    void Init(float InDamageRadius, float InDamage, float InImpulse, float InHalfHeight);

    float GetHalfHeight() const { return HalfHeight; }

private:

    UPROPERTY(EditAnywhere)
    float HalfHeight;
};
