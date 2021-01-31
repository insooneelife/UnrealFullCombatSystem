// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AITeleportAbility.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UParticleSystem;
class USoundBase;
class UEnvQuery;
/**
 * 
 */
UCLASS()
class AAITeleportAbility : public AAIAbilityBase
{
	GENERATED_BODY()
	
public:
    AAITeleportAbility();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Released() override;
    virtual void Effect() override;

private:
    UFUNCTION()
    void OnQueryFinished(
        UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

    void SpawnTeleportParticle();

    void MakeOwnerUntargetable(float InDuration);

    void PlaySound();

    bool AbilityTeleport();

private:
    UPROPERTY(EditAnywhere, Category = LoadedObject)
    UParticleSystem* TeleportParticle;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    USoundBase* Sound;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    UEnvQuery* Query;
};
