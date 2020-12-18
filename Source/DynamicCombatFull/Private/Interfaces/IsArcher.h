// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsArcher.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsArcher : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIsArcher
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual float GetAimAlpha() const = 0;
    virtual bool DoesHoldBowString() const = 0;
    virtual FName GetBowStringSocketName() const = 0;

};
