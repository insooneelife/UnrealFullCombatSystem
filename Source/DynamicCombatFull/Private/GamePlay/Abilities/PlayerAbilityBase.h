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
    APlayerAbilityBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    FTransform GetSurfaceTransform(float Range, float MaxSurfaceAngle, float MaxHeightDeviation) const;
    FTransform GetCrosshairTransform(FName SpawnSocket) const;
    FTransform GetBeamTransform(float Range, float Radius, FName SpawnSocket, bool bGoesThroughObjects)const;

    ABaseCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

private:
    UPROPERTY()
    ABaseCharacter* PlayerCharacter;

};
