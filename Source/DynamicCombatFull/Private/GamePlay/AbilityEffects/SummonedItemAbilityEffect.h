// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "SummonedItemAbilityEffect.generated.h"

class UEquipmentComponent;
class UStateManagerComponent;
class UItemBase;

UCLASS()
class ASummonedItemAbilityEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASummonedItemAbilityEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void UnsummonWeapon();
    void SummonWeapon();
    void AttemptToUnsummon();
    void StoreItemBeforeSummon();
    void EquipOldItem();
    int GetActiveIndex() const;

    EItemType GetCreatedItemType() const;
    bool IsItemInInventory(const FStoredItem& Item) const;

public:

    UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
    UStateManagerComponent* GetStateManager() const { return StateManager; }
    TSubclassOf<UItemBase> GetItemClass() const { return ItemClass; }
    float GetDuration()const { return Duration; }
    int GetAmount() const { return Amount; }
    const FStoredItem& GetSummonedItem() const { return SummonedItem; }
    int GetItemIndex() const { return ItemIndex; }
    int GetSlotIndex()const { return SlotIndex; }
    float GetElapsedTime()const { return ElapsedTime; }
    bool IsAutoSwap() const { return bAutoSwap; }
    const FStoredItem& GetItemBeforeSummon() const { return ItemBeforeSummon; }

    void SetItemClass(TSubclassOf<UItemBase> InClass) { ItemClass = InClass; }
    void SetDuration(float InDuration) { Duration = InDuration; }
    void SetAmount(int InAmount) { Amount = InAmount; }
    void SetAutoSwap(bool bInAutoSwap) { bAutoSwap = bInAutoSwap; }

private:
    UPROPERTY()
        UEquipmentComponent* EquipmentComponent;

    UPROPERTY()
        UStateManagerComponent* StateManager;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UItemBase> ItemClass;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(EditAnywhere)
    int Amount;

    FStoredItem SummonedItem;

    int ItemIndex;

    int SlotIndex;

    float ElapsedTime;

    UPROPERTY(EditAnywhere)
    bool bAutoSwap;

    FStoredItem ItemBeforeSummon;
};
