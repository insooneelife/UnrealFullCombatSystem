// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "CanMeleeAttack.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanMeleeAttack : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;
class AActor;
class UMontageManagerComponent;
/**
 * 
 */
class ICanMeleeAttack
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual ACharacter* GetThisCharacter() = 0;
    virtual EMeleeAttackType GetMeleeAttackType() const = 0;
    virtual void SetMeleeAttackCounter(int Value) = 0;
    virtual int GetMeleeAttackCounter() const = 0;
    virtual bool CanMeleeAttack() const = 0;
    virtual float GetMeleeDamage() const = 0;
    virtual float MeleeAttack(EMeleeAttackType InType) = 0;

    FHitData MakeMeleeHitData(AActor* HitActor);

    void ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower);

    UAnimMontage* GetNextMeleeAttackMontage(
        UMontageManagerComponent* MontageManager, EMeleeAttackType AttackType);

    void ResetMeleeAttackCounter() { SetMeleeAttackCounter(0); }

    // weapon collision events
    UFUNCTION()
    virtual void OnHit(const FHitResult& InHit);
};
