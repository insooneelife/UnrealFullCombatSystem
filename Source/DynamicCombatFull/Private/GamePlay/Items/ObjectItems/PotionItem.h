// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GameCore/DataTypes.h"
#include "Interfaces/ItemHasUseMontage.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "PotionItem.generated.h"

class UAnimMontage;
UCLASS(Blueprintable)
class UPotionItem : public UItemBase, public IItemHasUseMontage, public IItemIsDisplayed
{
	GENERATED_BODY()
public:

    UPotionItem(const FObjectInitializer& ObjectInitializer);

    virtual UAnimMontage* GetUseMontage() const override { return UseMontage; }

    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

    void UseItem(AActor* Caller);

private:
    UPROPERTY(EditAnywhere, Category = "Display")
    TSubclassOf<ADisplayedItem> DisplayedItemClass;

    UPROPERTY(EditAnywhere, Category = "Potion")
    UAnimMontage* UseMontage;

    UPROPERTY(EditAnywhere, Category = "Potion")
    EStat Type;

    UPROPERTY(EditAnywhere, Category = "Potion")
    float Value;
};
