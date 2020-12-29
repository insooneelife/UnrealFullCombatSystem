// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AbilityBase.h"
#include "AIAbilityBase.generated.h"

class AAICharacter;
class UAbilityComponent;
/**
 * 
 */
UCLASS()
class AAIAbilityBase : public AAbilityBase
{
	GENERATED_BODY()
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    virtual void Init(UAbilityComponent* InAbilityComponent) override;

    FVector GetTargetLoc() const;
    AActor* GetTarget() const;
    FTransform GetSurfaceTargetTransform(float Range) const;
    FTransform GetSurfacePredictedTargetTransform(float Range) const;

private:
    UPROPERTY()
        AAICharacter* AICharacter;
};
