// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIsInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual void Interact(AActor* Caller) = 0;
    virtual FName GetInteractionMessage() const = 0;
};
