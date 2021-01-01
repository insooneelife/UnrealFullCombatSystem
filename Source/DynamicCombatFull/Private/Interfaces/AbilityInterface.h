// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IAbilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "CanCastAbility", ScriptName = "CanCastAbility"))
    bool K2_CanCastAbility() const;

    virtual bool K2_CanCastAbility_Implementation() const = 0;

    virtual bool CanCastAbility() const = 0;
    virtual float GetMagicDamage() const = 0;
    virtual float GetCastingSpeed() const = 0;
};
