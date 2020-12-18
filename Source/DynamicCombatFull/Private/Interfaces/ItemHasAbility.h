// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemHasAbility.generated.h"

class AAbilityBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemHasAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IItemHasAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual TSubclassOf<AAbilityBase> GetAbility() const = 0;
};
