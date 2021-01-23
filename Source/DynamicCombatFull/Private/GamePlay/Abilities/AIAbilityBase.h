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
public:
    AAIAbilityBase();

protected:
    virtual void NativeInit(UAbilityComponent* InAbilityComponent) override;

public:

    UFUNCTION(BlueprintCallable)
    FVector GetTargetLoc() const;

    UFUNCTION(BlueprintCallable)
    AActor* GetTarget() const;

    UFUNCTION(BlueprintCallable)
    FTransform GetSurfaceTargetTransform(float InRange) const;

    UFUNCTION(BlueprintCallable)
    FTransform GetSurfacePredictedTargetTransform(float InRange) const;

    UFUNCTION(BlueprintCallable)
    AAICharacter* GetAICharacter() const { return AICharacter.Get(); }

private:

    TWeakObjectPtr<AAICharacter> AICharacter;
};
