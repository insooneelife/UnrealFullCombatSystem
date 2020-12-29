// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemIsDisplayed.h"

#include "ShieldItem.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class UShieldItem
    : public UItemBase, public IItemCanBlock, public IItemIsDisplayed
{
    GENERATED_BODY()
public:
    UShieldItem(const FObjectInitializer& ObjectInitializer);

    virtual float GetBlockValue() const override { return BlockValue; }

    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

private:

    UPROPERTY(EditAnywhere, Category = "Shield")
    float BlockValue;

    UPROPERTY(EditAnywhere, Category = "Display")
    TSubclassOf<ADisplayedItem> DisplayedItemClass;
};
