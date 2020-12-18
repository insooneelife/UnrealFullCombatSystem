// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemHasUseMontage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemHasUseMontage : public UInterface
{
	GENERATED_BODY()
};


class UAnimMontage;
/**
 * 
 */
class IItemHasUseMontage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual UAnimMontage* GetUseMontage() const = 0;
};
