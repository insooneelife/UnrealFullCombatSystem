// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "Components/InventoryComponent.h"
#include "GameCore/CustomStructs.h"
#include "GameCore/DCSGameMode.h"
#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInCombatChangedSignature, bool, bInIsInCombat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FItemInSlotChangedSignature, FStoredItem, InOldItem, FStoredItem, InNewItem, EItemType, InType, int, InSlotIndex, int, InActiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FActiveItemChangedSignature, FStoredItem, InOldItem, FStoredItem, InNewItem, EItemType, InType, int, InSlotIndex, int, InActiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSlotHiddenChangedSignature, EItemType, InSlotType, int, InSlotIndex, FStoredItem, InActiveItem, bool, bInIsHidden);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMainHandTypeChangedSignature, EItemType, InType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatTypeChangedSignature, ECombatType, InCombatType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponTypeChangedSignature, EWeaponType, InWeaponType);

class ADisplayedItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    UFUNCTION(BlueprintCallable)
    ADisplayedItem* GetDisplayedItem(EItemType InType, int InSlotIndex) const;

    UFUNCTION(BlueprintCallable)
    FStoredItem GetActiveItem(EItemType InType, int InSlotIndex) const;

    UFUNCTION(BlueprintCallable)
    bool IsSlotHidden(EItemType InType, int InSlotIndex) const;

    UFUNCTION(BlueprintCallable)
    bool IsShieldEquipped() const;

    UFUNCTION(BlueprintCallable)
    bool IsInCombat() const;

    UFUNCTION(BlueprintCallable)
    void ToggleCombat();

    UFUNCTION(BlueprintCallable)
    bool IsTwoHandedWeaponEquipped() const;

public:
    void Init();
    void SwitchMainHandType(bool bInForward);
    void SwitchSlotActiveIndex(EItemType InType, int InSlotIndex, bool bInForward, bool bInIgnoreEmptyItems);
    void SetMainHandType(EItemType InType);
    void SetSlotActiveIndex(EItemType InType, int InSlotIndex, int InNewActiveIndex);
    FStoredItem GetItemInSlot(EItemType InType, int InSlotIndex, int InItemIndex) const;
    void UpdateItemInSlot(
        EItemType InType, 
        int InSlotIndex, 
        int InItemIndex, 
        FStoredItem InItem, 
        EHandleSameItemMethod InHandleSameItemMethod);

    int GetEqSlotsIndex(EItemType InType) const;
    bool IsActiveItemIndex(EItemType InType, int InSlotIndex, int InItemIndex) const;
    bool IsItemEquipped(FGuid InItemId) const;
    bool IsItemActive(FGuid InItemId) const;
    bool AreArrowsEquipped() const;
    bool CanBlock() const;
    bool GetBlockValue(float& InValue) const;
    bool IsWeaponEquipped() const;
    void SetCombat(bool bInValue);

    const TArray<FEquipmentSlots>& GetEquipmentSlots() const { return EquipmentSlots; }
    void SetEquipmentSlots(const TArray<FEquipmentSlots>& InEquipmentSlots) { EquipmentSlots = InEquipmentSlots; }
    EItemType GetSelectedMainHandType() const { return SelectedMainHandType; }
    ECombatType GetCombatType() const { return CombatType; }
    EWeaponType GetWeaponType() const { return WeaponType; }

protected:
    UFUNCTION()
    void OnItemModified(FStoredItem InItem);

    UFUNCTION()
    void OnGameLoaded();

private:

    void UpdateDisplayedItem(EItemType InType, int InSlotIndex);

    void SetSlotHidden(EItemType InType, int InSlotIndex, bool bInIsHidden);

    void BroadcastOnItemInSlotChanged(
        FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex);

    void ActiveItemChanged(
        FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex);
    
    void UseActiveItemAtSlot(EItemType InType, int InSlotIndex);
    bool FindItem(FStoredItem InItem, EItemType& OutType, int& OutSlotIndex, int& OutItemIndex) const;
    void BuildEquipment(const TArray<FEquipmentSlots>& InEquipment);
    
    void UpdateCombatType();
    void SetItemInSlot(EItemType InType, int InSlotIndex, int InItemIndex, FStoredItem InItem);
    void AttachDisplayedItem(EItemType InType, int InSlotIndex);

    FStoredItem GetWeapon() const;
    EItemType GetItemType(FStoredItem InItem) const;
    bool IsItemValid(FStoredItem InItem) const;
    bool IsItemIndexValid(EItemType InType, int InSlotIndex, int InItemIndex) const;

    int GetActiveItemIndex(EItemType InType, int InSlotIndex) const;
    bool IsSlotIndexValid(EItemType InType, int InSlotIndex) const;
    template <typename ElementType>
    int GetNextArrayIndex(TArray<ElementType> InArray, int InIndex, bool bInForward) const;
    bool IsItemTwoHanded(FStoredItem InItem) const;

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

    UPROPERTY(EditAnywhere)
    TArray<FEquipmentSlots> EquipmentSlots;

    UPROPERTY(EditAnywhere)
    TMap<EItemType, FDisplayedItems> DisplayedItems;

    TArray<EItemType> MainHandTypes;

    EItemType SelectedMainHandType;

    bool bIsInCombat;

    ECombatType CombatType;

    EWeaponType WeaponType;

    TArray<FGuid> EquippedItems;

    TArray<FGuid> ActiveItems;
};
