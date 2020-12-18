// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GameCore/DataTypes.h"
#include "Interfaces/ItemHasUseMontage.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "PotionItem.generated.h"

class UAnimMontage;
UCLASS()
class UPotionItem : public UItemBase, public IItemHasUseMontage, public IItemIsDisplayed
{
	GENERATED_BODY()
public:

    UPotionItem(const FObjectInitializer& ObjectInitializer);

    virtual UAnimMontage* GetUseMontage() const override { return UseMontage; }

    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

    void UseItem(AActor* Caller);

private:
    UPROPERTY(EditAnywhere)
    UAnimMontage* UseMontage;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADisplayedItem> DisplayedItemClass;

    UPROPERTY(EditAnywhere)
    EStat Type;

    UPROPERTY(EditAnywhere)
    float Value;
};
