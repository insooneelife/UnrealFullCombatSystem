// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "Components/InventoryComponent.h"
#include "GameCore/CustomStructs.h"
#include "GameCore/DCSGameMode.h"
#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInCombatChangedSignature, bool, bIsInCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FItemInSlotChangedSignature, FStoredItem, OldItem, FStoredItem, NewItem, EItemType, Type, int, SlotIndex, int, ActiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FActiveItemChangedSignature, FStoredItem, OldItem, FStoredItem, NewItem, EItemType, Type, int, SlotIndex, int, ActiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSlotHiddenChangedSignature, EItemType, SlotType, int, SlotIndex, FStoredItem, ActiveItem, bool, bIsHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMainHandTypeChangedSignature, EItemType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatTypeChangedSignature, ECombatType, CombatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponTypeChangedSignature, EWeaponType, WeaponType);

class ADisplayedItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

    void Init();

    UFUNCTION()
    void OnItemModified(FStoredItem InItem);

    UFUNCTION()
    void OnGameLoaded();

    void UpdateDisplayedItem(EItemType Type, int SlotIndex);

    ADisplayedItem* GetDisplayedItem(EItemType Type, int SlotIndex) const;

    bool IsItemEquipped(FGuid ItemId) const;

    FStoredItem GetActiveItem(EItemType Type, int SlotIndex) const;

    bool IsSlotHidden(EItemType Type, int SlotIndex) const;

    void SetSlotHidden(EItemType Type, int SlotIndex, bool bIsHidden);

    void UpdateItemInSlot(
        EItemType Type, int SlotIndex, int ItemIndex, FStoredItem Item, EHandleSameItemMethod HandleSameItemMethod);

    void BroadcastOnItemInSlotChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex);

    int GetEqSlotsIndex(EItemType Type) const;

    // ChangeActiveItem
    void ActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex);

    void SwitchSlotActiveIndex(EItemType Type, int SlotIndex, bool Forward, bool bIgnoreEmptyItems);

    void SwitchMainHandType(bool bForward);

    FStoredItem GetWeapon() const;

    EItemType GetSelectedMainHandType() const;

    bool IsItemActive(FGuid ItemId) const;

    bool IsActiveItemIndex(EItemType Type, int SlotIndex, int ItemIndex);

    void UseActiveItemAtSlot(EItemType Type, int SlotIndex);

    bool FindItem(FStoredItem Item, EItemType& OutType, int& OutSlotIndex, int& OutItemIndex);

    void BuildEquipment(const TArray<FEquipmentSlots>& Equipment);

    FStoredItem GetItemInSlot(EItemType Type, int SlotIndex, int ItemIndex) const;

    void SetSlotActiveIndex(EItemType Type, int SlotIndex, int NewActiveIndex);

    void SetMainHandType(EItemType Type);

public:

    bool IsShieldEquipped() const;

    bool IsInCombat() const;

    void ToggleCombat();

    void SetCombat(bool bValue);

    ECombatType GetCombatType() const;

    bool CanBlock() const;

    void UpdateCombatType();

    bool GetBlockValue(float& Value) const;

    bool AreArrowsEquipped() const;

    bool IsTwoHandedWeaponEquipped() const;

    EWeaponType GetWeaponType() const;

    bool IsWeaponEquipped() const;

    const TArray<FEquipmentSlots>& GetEquipmentSlots() const;

private:
    EItemType GetItemType(FStoredItem Item) const;

    bool IsItemValid(FStoredItem Item) const;

    bool IsItemIndexValid(EItemType Type, int SlotIndex, int ItemIndex) const;

    void SetItemInSlot(EItemType Type, int SlotIndex, int ItemIndex, FStoredItem Item);

    int GetActiveItemIndex(EItemType Type, int SlotIndex) const;

    bool IsSlotIndexValid(EItemType Type, int SlotIndex) const;

    template <typename ElementType>
    int GetNextArrayIndex(TArray<ElementType> Array, int Index, bool bForward) const;

    void AttachDisplayedItem(EItemType Type, int SlotIndex);

    bool IsItemTwoHanded(FStoredItem Item) const;

public:
    UPROPERTY(BlueprintAssignable)
        FInCombatChangedSignature OnInCombatChanged;

    UPROPERTY(BlueprintAssignable)
        FItemInSlotChangedSignature OnItemInSlotChanged;

    UPROPERTY(BlueprintAssignable)
        FActiveItemChangedSignature OnActiveItemChanged;

    UPROPERTY(BlueprintAssignable)
        FSlotHiddenChangedSignature OnSlotHiddenChanged;

    UPROPERTY(BlueprintAssignable)
        FMainHandTypeChangedSignature OnMainHandTypeChanged;

    UPROPERTY(BlueprintAssignable)
        FCombatTypeChangedSignature OnCombatTypeChanged;

    UPROPERTY(BlueprintAssignable)
        FWeaponTypeChangedSignature OnWeaponTypeChanged;

private:

    UPROPERTY()
    UInventoryComponent* Inventory;

    TMap<EItemType, FDisplayedItems> DisplayedItems;

    UPROPERTY()
    TArray<FEquipmentSlots> EquipmentSlots;

    TArray<EItemType> MainHandTypes;

    EItemType SelectedMainHandType;

    bool bIsInCombat;

    ECombatType CombatType;

    EWeaponType WeaponType;

    TArray<FGuid> EquippedItems;

    TArray<FGuid> ActiveItems;
};
