// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "GameCore/CustomStructs.h"
#include "HandDisplayedItem.generated.h"

/**
 * 
 */
UCLASS()
class AHandDisplayedItem : public ADisplayedItem
{
	GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual FName GetAttachmentSocket() const override;

    void OnSlotHiddenChanged(EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden);


private:
    FName HandAttachmentSocket;
};
