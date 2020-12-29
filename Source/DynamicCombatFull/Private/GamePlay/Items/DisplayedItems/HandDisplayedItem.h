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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void Init(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex) override;

    virtual FName GetAttachmentSocket() const override;

    UFUNCTION()
    void OnSlotHiddenChanged(EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden);


protected:
    UPROPERTY(EditAnywhere, Category = "Sockets")
    FName HandAttachmentSocket;
};
