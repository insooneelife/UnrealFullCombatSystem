// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "Interfaces/ItemHasModifiers.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "GameCore/CustomStructs.h"
#include "GameCore/DataTypes.h"
#include "Engine/StaticMesh.h"
#include "ArrowItem.generated.h"

class AArrowProjectileBase;
/**
 *
 */
UCLASS(Blueprintable)
class UArrowItem : public UItemBase, public IItemHasModifiers, public IItemIsDisplayed
{
    GENERATED_BODY()
public:

    UArrowItem(const FObjectInitializer& ObjectInitializer);

    void Init(UStaticMesh* InArrowMesh) { ArrowMesh = InArrowMesh; }

    virtual const TArray<FModifier>& GetModifiers() const override { return Modifiers; }
    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

    UStaticMesh* GetArrowMesh() const { return ArrowMesh; }
    TSubclassOf<AArrowProjectileBase> GetProjectile() const { return Projectile; }
private:

    UPROPERTY(EditAnywhere, Category = "Arrow")
    UStaticMesh* ArrowMesh;

    UPROPERTY(EditAnywhere, Category = "Arrow")
    TSubclassOf<AArrowProjectileBase> Projectile;

    UPROPERTY(EditAnywhere, Category = "Modifiers")
    TArray<FModifier> Modifiers;

    UPROPERTY(EditAnywhere, Category = "Display")
    TSubclassOf<ADisplayedItem> DisplayedItemClass;


};