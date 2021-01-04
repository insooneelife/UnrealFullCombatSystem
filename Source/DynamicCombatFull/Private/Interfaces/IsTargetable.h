// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsTargetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsTargetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIsTargetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual void OnSelected() = 0;
    virtual void OnDeselected() = 0;
    virtual bool IsTargetable() const = 0;
};
