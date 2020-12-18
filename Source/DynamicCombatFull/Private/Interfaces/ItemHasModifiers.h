// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/CustomStructs.h"
#include "ItemHasModifiers.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemHasModifiers : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IItemHasModifiers
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual const TArray<FModifier>& GetModifiers() const = 0;
};
