// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/WeaponItem.h"
#include "Interfaces/ItemHasAbility.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "SpellWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class USpellWeaponItem 
    :
    public UWeaponItem,
    public IItemHasAbility,
    public IItemIsDisplayed
{
	GENERATED_BODY()
public:
    USpellWeaponItem(const FObjectInitializer& ObjectInitializer);

    virtual TSubclassOf<AAbilityBase> GetAbility() const override { return AbilityClass; }

    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

private:

    UPROPERTY(EditAnywhere)
    TSubclassOf<AAbilityBase> AbilityClass;

    // Blueprint'/Game/DynamicCombatSystem/Blueprints/Items/DisplayedItems/DI_SpellBP.DI_SpellBP'
    UPROPERTY(EditAnywhere)
    TSubclassOf<ADisplayedItem> DisplayedItemClass;
};
