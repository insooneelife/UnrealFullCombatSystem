// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "Interfaces/ItemHasModifiers.h"
#include "GameCore/CustomStructs.h"
#include "GameCore/DataTypes.h"
#include "RingItem.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class URingItem : public UItemBase, public IItemHasModifiers
{
    GENERATED_BODY()
public:

    URingItem(const FObjectInitializer& ObjectInitializer);

    virtual const TArray<FModifier>& GetModifiers() const override { return Modifiers; }

private:

    UPROPERTY(EditAnywhere, Category = "Modifiers")
    TArray<FModifier> Modifiers;
};

