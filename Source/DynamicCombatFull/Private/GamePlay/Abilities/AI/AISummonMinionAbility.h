// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AISummonMinionAbility.generated.h"

class USoundBase;
class UEnvQueryInstanceBlueprintWrapper;
class APawn;
class UEnvQuery;
/**
 * 
 */
UCLASS()
class AAISummonMinionAbility : public AAIAbilityBase
{
	GENERATED_BODY()
	
public:
    AAISummonMinionAbility();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Released() override;
    virtual void Effect() override;


private:
    UFUNCTION()
    void OnQueryFinished(
        UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

    void PlaySound();

    void SummonMinion();

private:

    UPROPERTY(EditAnywhere, Category = LoadedClass)
    TSubclassOf<APawn> SpawnedMinionClass;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    USoundBase* SoundObject;

    UPROPERTY(EditAnywhere, Category = LoadedObject)
    UEnvQuery* QueryObject;
};
