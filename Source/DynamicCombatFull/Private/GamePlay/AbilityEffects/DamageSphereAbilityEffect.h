// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageAbilityEffectBase.h"
#include "DamageSphereAbilityEffect.generated.h"

UCLASS()
class ADamageSphereAbilityEffect : public ADamageAbilityEffectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageSphereAbilityEffect();

protected:

public:
    virtual void GetTraceObjects(TArray<FHitResult>& OutHits) const override;

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Init", ScriptName = "Init"))
    void K2_Init(float InDamageRadius, float InDamage, float InImpulse) 
    {
        Super::Init(InDamageRadius, InDamage, InImpulse, true); 
    }

private:


};
