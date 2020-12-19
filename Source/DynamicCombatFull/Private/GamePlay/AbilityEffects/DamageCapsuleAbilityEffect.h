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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    virtual void GetTraceObjects(TArray<FHitResult>& OutHits) const override;

    float GetHalfHeight() const { return HalfHeight; }

private:

    UPROPERTY(EditAnywhere)
    float HalfHeight;
};
