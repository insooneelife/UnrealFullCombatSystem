// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AbilityBase.h"
#include "PlayerAbilityBase.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS()
class APlayerAbilityBase : public AAbilityBase
{
	GENERATED_BODY()
public:
    APlayerAbilityBase() {}

protected:
    virtual void NativeInit(UAbilityComponent* InAbilityComponent) override;

public:

    UFUNCTION(BlueprintCallable)
    FTransform GetSurfaceTransform(float Range, float MaxSurfaceAngle, float MaxHeightDeviation) const;

    UFUNCTION(BlueprintCallable)
    FTransform GetCrosshairTransform(FName SpawnSocket) const;

    UFUNCTION(BlueprintCallable)
    FTransform GetBeamTransform(float Range, float Radius, FName SpawnSocket, bool bGoesThroughObjects)const;

    UFUNCTION(BlueprintCallable)
    ABaseCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

private:
    UPROPERTY()
    ABaseCharacter* PlayerCharacter;

};
