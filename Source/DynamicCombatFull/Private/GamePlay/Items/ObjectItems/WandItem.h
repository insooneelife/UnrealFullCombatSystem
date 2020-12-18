// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "Interfaces/ItemHasModifiers.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "WandItem.generated.h"

/**
 * 
 */
UCLASS()
class UWandItem 
    : 
    public UItemBase,
    public IItemHasModifiers,
    public IItemIsDisplayed
{
	GENERATED_BODY()
public:
    UWandItem(const FObjectInitializer& ObjectInitializer);

    virtual const TArray<FModifier>& GetModifiers() const override { return Modifiers; }
    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

private:

    UPROPERTY(EditAnywhere)
    TArray<FModifier> Modifiers;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADisplayedItem> DisplayedItemClass;
};
