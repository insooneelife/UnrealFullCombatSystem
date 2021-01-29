// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/DataTypes.h"
#include "CanGetEffects.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanGetEffects : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class ICanGetEffects
{
    GENERATED_BODY()

        // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual bool CanEffectBeApplied(EEffectType Type, const AActor* const Applier) const;

    virtual bool CanBeAttacked() const = 0;

    virtual bool CanBeStunned() const = 0;

    virtual bool CanBeInterrupted() const = 0;

    virtual bool CanBeBackstabbed() const = 0;
};