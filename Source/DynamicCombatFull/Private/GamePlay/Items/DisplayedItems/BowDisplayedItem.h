// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "GameCore/CustomStructs.h"
#include "HandDisplayedItem.h"
#include "BowDisplayedItem.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class ABowDisplayedItem : public AHandDisplayedItem
{
	GENERATED_BODY()

public:
    ABowDisplayedItem();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex) override;

public:

    virtual void SimulatePhysics() override;

    virtual bool Attach();

    virtual UPrimitiveComponent* GetPrimaryComponent() const;

    UFUNCTION()
    void OnActiveItemChanged(
        FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex);
    
    UFUNCTION(BlueprintCallable)
    void UpdateArrowAttachment(bool bAttachToOwner);

    UFUNCTION(BlueprintCallable)
    void UpdateArrowVisibility(bool bVisible);
    void UpdateArrowMesh();

private:
    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* ArrowMesh;

    UPROPERTY(EditAnywhere)
        USkeletalMeshComponent* BowMesh;


    UPROPERTY(EditAnywhere, Category = "Sockets")
    FName ArrowSocketName;

    UPROPERTY(EditAnywhere)
    FStoredItem LastUpdatedArrow;
};
