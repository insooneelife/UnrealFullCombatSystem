// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/DataTypes.h"
#include "MontageAction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMontageAction : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;
class UMontageManagerComponent;
class UEquipmentComponent;
/**
 * 
 */
class IMontageAction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual UEquipmentComponent* GetEquipment() const = 0;
    virtual UMontageManagerComponent* GetMontageManager() const = 0;
    virtual UAnimMontage* GetStunMontage(EDirection Direction) const;
    virtual UAnimMontage* GetBlockMontage() const;
    virtual UAnimMontage* GetImpactMontage() const;
    virtual UAnimMontage* GetParriedMontage() const;
    virtual UAnimMontage* GetParryMontage() const;

};
