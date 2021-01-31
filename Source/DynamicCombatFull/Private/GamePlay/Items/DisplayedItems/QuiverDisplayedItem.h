// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "GameCore/CustomStructs.h"
#include "QuiverDisplayedItem.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class AQuiverDisplayedItem : public ADisplayedItem
{
	GENERATED_BODY()
	
public:
    AQuiverDisplayedItem();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
    void UpdateQuiver();

protected:
    virtual void NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex) override;

    UFUNCTION()
    void OnActiveItemChanged(
        const FStoredItem& InOldItem,
        const FStoredItem& InNewItem,
        EItemType InType,
        int InSlotIndex,
        int InActiveIndex);

    void UpdateArrowMesh();

private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere)
    UInstancedStaticMeshComponent* InstancedArrowsMesh;

    UPROPERTY(EditAnywhere)
    TArray<USceneComponent*> ArrowSpawns;

    FStoredItem LastUpdatedArrow;
};
