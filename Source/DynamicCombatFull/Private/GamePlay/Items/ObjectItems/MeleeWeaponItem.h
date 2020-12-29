// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GamePlay/Items/ObjectItems/WeaponItem.h"
#include "Interfaces/ItemCanBeTwoHanded.h"
#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemHasModifiers.h"
#include "Interfaces/ItemHasAbility.h"
#include "Interfaces/ItemIsDisplayed.h"

#include "MeleeWeaponItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UMeleeWeaponItem 
    : 
    public UWeaponItem, 
    public IItemCanBeTwoHanded, 
    public IItemCanBlock, 
    public IItemHasModifiers, 
    public IItemHasAbility, 
    public IItemIsDisplayed
{
	GENERATED_BODY()
public:
    UMeleeWeaponItem(const FObjectInitializer& ObjectInitializer);

    virtual bool IsTwoHanded() const override { return bIsTwoHanded; }
    virtual float GetBlockValue() const override { return BlockValue; }

    virtual const TArray<FModifier>& GetModifiers() const override { return Modifiers; }

    virtual TSubclassOf<AAbilityBase> GetAbility() const override { return AbilityClass; }

    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

private:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    bool bIsTwoHanded;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    float BlockValue;

    UPROPERTY(EditAnywhere, Category = "Modifiers")
    TArray<FModifier> Modifiers;

    UPROPERTY(EditAnywhere, Category = "Ability")
    TSubclassOf<AAbilityBase> AbilityClass;

    UPROPERTY(EditAnywhere, Category = "Display")
    TSubclassOf<ADisplayedItem> DisplayedItemClass;
};
