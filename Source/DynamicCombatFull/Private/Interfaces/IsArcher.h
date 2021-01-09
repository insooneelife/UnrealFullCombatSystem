// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "IsArcher.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UIsArcher : public UInterface
{
	GENERATED_BODY()
};

class ACharacter;
class UEquipmentComponent;
class UStatsManagerComponent;
/**
 * 
 */
class IIsArcher
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual ACharacter* GetThisCharacter() = 0;
    virtual UEquipmentComponent* GetEquipment() const = 0;
    virtual UStatsManagerComponent* GetStatsManager() const = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetAimAlpha", ScriptName = "GetAimAlpha"))
        float K2_GetAimAlpha() const;
    virtual float K2_GetAimAlpha_Implementation() const = 0;

    virtual float GetAimAlpha() const = 0;
    virtual float GetArrowInitialSpeed() const = 0;
    virtual bool DoesHoldBowString() const = 0;
    virtual float BowAttack() = 0;

    virtual FTransform GetSpawnArrowTransform() const = 0;

    void SpawnArrowProjectile(const FStoredItem& Item);
    FName GetBowStringSocketName() const;
    bool CanBowAttack() const;
    float GetRangeDamage() const;
};
