// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/CustomStructs.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemAddedSignature, FStoredItem, InAddedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemovedSignature, FStoredItem, InRemainedItem);

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

public:
    UFUNCTION(BlueprintCallable)
    void UseItem(FGuid InItemId);

public:
    int FindIndexByClass(TSubclassOf<UItemBase> InItemClass) const;

    int FindIndexById(FGuid InItemId) const;

    FStoredItem GetItemAtIndex(int InIndex) const;

    void RemoveItemAtIndex(int InIndex, int InAmount);

    void AddItem(TSubclassOf<UItemBase> InItemClass, int InAmount);

    void DropItem(FStoredItem InItem);

    const TArray<FStoredItem>& GetInventory() const { return Inventory; }
    void SetInventory(const TArray<FStoredItem>& InInventory) { Inventory = InInventory; }

protected:
    UFUNCTION()
    void OnGameLoaded();

private:
    void ClearInventory();

    bool IsSlotEmpty(int InIndex) const;

    bool IsItemValid(FStoredItem InItem) const;

public:
    UPROPERTY(BlueprintAssignable)
    FItemAddedSignature OnItemAdded;

    UPROPERTY(BlueprintAssignable)
    FItemRemovedSignature OnItemRemoved;

private:

    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<APickupActor> SpawnPickupActorClass;

    UPROPERTY(EditAnywhere)
    TArray<FStoredItem> Inventory;

};