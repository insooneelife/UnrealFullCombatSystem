// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/CustomStructs.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedSignature, FStoredItem, AddedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedSignature, FStoredItem, RemainedItem);

class APickupActor;
class UItemBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventoryComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
        void OnGameLoaded();

public:
    void AddItem(TSubclassOf<UItemBase> InItemClass, int InAmount);
    //void RemoveItem(TSubclassOf<UItemBase> InItemClass, int InAmount);

    void ClearInventory();
    void RemoveItemAtIndex(int Index, int InAmount);

    void DropItem(FStoredItem InItem);

    UFUNCTION(BlueprintCallable)
    void UseItem(FGuid ItemId);

    FStoredItem GetItemAtIndex(int Index) const;

    int FindIndexByClass(TSubclassOf<UItemBase> InItemClass) const;

    int FindIndexById(FGuid Id) const;

    bool IsSlotEmpty(int Index) const;

    bool IsItemValid(FStoredItem Item) const;

public:
    UPROPERTY(BlueprintAssignable)
        FItemAddedSignature OnItemAdded;

    UPROPERTY(BlueprintAssignable)
        FItemRemovedSignature OnItemRemoved;

public:
    const TArray<FStoredItem>& GetInventory() const { return Inventory; }
    void SetInventory(const TArray<FStoredItem>& InInventory) { Inventory = InInventory; }

private:

    UPROPERTY(EditAnywhere)
        TSubclassOf<APickupActor> SpawnPickupActorClass;

    UPROPERTY(EditAnywhere)
    TArray<FStoredItem> Inventory;

};