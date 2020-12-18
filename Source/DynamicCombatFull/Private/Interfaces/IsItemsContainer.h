// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCore/DataTypes.h"
#include "Blueprint/UserWidget.h"
#include "IsItemsContainer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsItemsContainer : public UInterface
{
	GENERATED_BODY()
};

class UInventoryItemUI;
/**
 * 
 */
class IIsItemsContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual bool IsOpen() const = 0;
    virtual bool ItemClicked(UInventoryItemUI* Widget) const = 0;
    virtual bool UpdateItemsSlots(EItemType Type) = 0;
    virtual bool CreateItemsSlots() = 0;
};
