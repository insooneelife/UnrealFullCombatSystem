// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "GameCore/CustomStructs.h"
#include "BowDisplayedItem.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class ABowDisplayedItem : public ADisplayedItem
{
	GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void SimulatePhysics() override;

    virtual bool Attach();

    virtual UPrimitiveComponent* GetPrimaryComponent() const;

    void OnActiveItemChanged(
        FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex);
    
    void UpdateArrowAttachment(bool bAttachToOwner);
    void UpdateArrowVisibility(bool bVisible);
    void UpdateArrowMesh();

private:
    UPROPERTY()
        UStaticMeshComponent* ArrowMesh;

    UPROPERTY()
        UStaticMeshComponent* BowMesh;


    UPROPERTY(EditAnywhere)
    FName ArrowSocketName;

    UPROPERTY(EditAnywhere)
    FStoredItem LastUpdatedArrow;
};
