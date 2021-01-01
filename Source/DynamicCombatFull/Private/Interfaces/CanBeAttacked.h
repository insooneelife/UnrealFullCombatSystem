// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/CustomStructs.h"
#include "CanBeAttacked.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UCanBeAttacked : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ICanBeAttacked
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TakeDamage", ScriptName = "TakeDamage"))
    bool K2_TakeDamage(const FHitData& InHitData, EAttackResult& OutResult);

    virtual bool K2_TakeDamage_Implementation(const FHitData& InHitData, EAttackResult& OutResult) = 0;

    virtual bool TakeDamage(const FHitData& InHitData, EAttackResult& OutResult) = 0;

    virtual bool IsAlive() const = 0;

    virtual FName GetHeadSocket() const = 0;
};
