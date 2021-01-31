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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void Init(
        TSubclassOf<UItemBase> InClass,
        float InDuration,
        int InAmount,
        bool bInAutoSwap);

    void UnsummonWeapon();
    void SummonWeapon();
    void AttemptToUnsummon();
    void StoreItemBeforeSummon();
    void EquipOldItem();
    int GetActiveIndex() const;

    EItemType GetCreatedItemType() const;
    bool IsItemInInventory(const FStoredItem& Item) const;

public:

    UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent.Get(); }
    UStateManagerComponent* GetStateManager() const { return StateManager.Get(); }
    TSubclassOf<UItemBase> GetItemClass() const { return ItemClass; }
    float GetDuration()const { return Duration; }
    int GetAmount() const { return Amount; }
    const FStoredItem& GetSummonedItem() const { return SummonedItem; }
    int GetItemIndex() const { return ItemIndex; }
    int GetSlotIndex()const { return SlotIndex; }
    float GetElapsedTime()const { return ElapsedTime; }
    bool IsAutoSwap() const { return bAutoSwap; }
    const FStoredItem& GetItemBeforeSummon() const { return ItemBeforeSummon; }

private:
    TWeakObjectPtr<UEquipmentComponent> EquipmentComponent;

    TWeakObjectPtr<UStateManagerComponent> StateManager;

    TSubclassOf<UItemBase> ItemClass;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(EditAnywhere)
    int Amount;

    FStoredItem SummonedItem;

    UPROPERTY(EditAnywhere)
    int ItemIndex;

    UPROPERTY(EditAnywhere)
    int SlotIndex;

    UPROPERTY(EditAnywhere)
    float ElapsedTime;

    UPROPERTY(EditAnywhere)
    bool bAutoSwap;

    FStoredItem ItemBeforeSummon;
};
