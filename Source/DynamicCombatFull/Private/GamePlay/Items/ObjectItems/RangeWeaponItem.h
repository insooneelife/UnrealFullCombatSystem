// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/WeaponItem.h"
#include "Interfaces/ItemCanBeTwoHanded.h"
#include "Interfaces/ItemHasAbility.h"
#include "Interfaces/ItemHasModifiers.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "RangeWeaponItem.generated.h"

class AAbilityBase;
class ADisplayedItem;
/**
 * 
 */
UCLASS()
class URangeWeaponItem 
    : 
    public UWeaponItem,
    public IItemCanBeTwoHanded,
    public IItemHasModifiers,
    public IItemHasAbility,  
    public IItemIsDisplayed
{
	GENERATED_BODY()
	
public:
    URangeWeaponItem(const FObjectInitializer& ObjectInitializer);

    virtual bool IsTwoHanded() const override { return bIsTwoHanded; }

    virtual const TArray<FModifier>& GetModifiers() const override { return Modifiers; }

    virtual TSubclassOf<AAbilityBase> GetAbility() const override { return Ability; }

    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

private:

    UPROPERTY(EditAnywhere)
    bool bIsTwoHanded;

    UPROPERTY(EditAnywhere)
    TArray<FModifier> Modifiers;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AAbilityBase> Ability;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADisplayedItem> DisplayedItemClass;

};
