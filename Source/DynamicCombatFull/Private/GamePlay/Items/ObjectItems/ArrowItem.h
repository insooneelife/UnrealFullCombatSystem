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

/**
 *
 */
UCLASS()
class UArrowItem : public UItemBase, public IItemHasModifiers, public IItemIsDisplayed
{
    GENERATED_BODY()
public:

    UArrowItem(const FObjectInitializer& ObjectInitializer);

    void Init(UStaticMesh* InArrowMesh) { ArrowMesh = InArrowMesh; }

    virtual const TArray<FModifier>& GetModifiers() const override { return Modifiers; }
    virtual TSubclassOf<ADisplayedItem> GetDisplayedItem() const override { return DisplayedItemClass; }

    UStaticMesh* GetArrowMesh() const { return ArrowMesh; }
    TSubclassOf<AActor> GetProjectile() const { return Projectile; }
private:

    UPROPERTY(EditAnywhere)
    UStaticMesh* ArrowMesh;

    // Blueprint'/Game/DynamicCombatSystem/Blueprints/Projectiles/BP_BasicArrowProjectile.BP_BasicArrowProjectile'
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> Projectile;

    UPROPERTY(EditAnywhere)
    TArray<FModifier> Modifiers;

    UPROPERTY(EditAnywhere)
    TSubclassOf<ADisplayedItem> DisplayedItemClass;


};