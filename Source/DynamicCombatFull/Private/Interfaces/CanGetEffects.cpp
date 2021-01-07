// Fill out your copyright notice in the Description page of Project Settings.


#include "CanGetEffects.h"

// Add default functionality here for any ICanGetEffects functions that are not pure virtual.


bool ICanGetEffects::CanEffectBeApplied(EEffectType Type, AActor* Applier)
{
    if (CanBeAttacked())
    {
        if (Type == EEffectType::Stun)
        {
            return CanBeStunned() && CanBeInterrupted();
        }
        else if (Type == EEffectType::Burning)
        {
            return true;
        }
        else if (Type == EEffectType::Backstab)
        {
            return CanBeBackstabbed() && CanBeInterrupted();
        }
        else if (Type == EEffectType::Impact)
        {
            return CanBeInterrupted();
        }
        else if (Type == EEffectType::Parried)
        {
            return CanBeInterrupted();
        }
    }

    return false;
}