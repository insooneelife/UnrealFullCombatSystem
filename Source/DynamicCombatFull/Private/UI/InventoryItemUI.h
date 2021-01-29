// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "InventoryItemUI.generated.h"

class UInventoryComponent;
class UEquipmentComponent;
class UItemsGridUI;
class UItemBase;

/**
 * 
 */
UCLASS()
class UInventoryItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
    UInventoryItemUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnClicked_SlotButton();

    UFUNCTION()
    void OnHovered_SlotButton();

    UFUNCTION()
    void OnUnhovered_SlotButton();

    UFUNCTION()
    void OnItemInSlotChanged(
        const FStoredItem& OldItem,
        const FStoredItem& NewItem,
        EItemType Type,
        int SlotIndex, 
        int ItemIndex);

    UFUNCTION()
    void OnActiveItemChanged(
        const FStoredItem& OldItem,
        const FStoredItem& NewItem,
        EItemType Type,
        int SlotIndex,
        int ItemIndex);

public:
    void Init(
        UInventoryComponent* const InInventoryComponent,
        UEquipmentComponent* const InEquipmentComponent,
        UItemsGridUI* const InItemsGridUI);

    void UpdateWidget(const FStoredItem& InItem);
    void UpdateAmountText();
    void UpdateIsEquippedImage();
    void SetFocusedImage(bool bVisible);

    bool IsOpen() const;

    FStoredItem GetItem() const { return Item; }


private:
    UPROPERTY()
    class UInventoryComponent* InventoryComponent;

    UPROPERTY()
    class UEquipmentComponent* EquipmentComponent;

    float SlotSize;

    FStoredItem Item;

    UPROPERTY()
    class UItemsGridUI* ItemsGridUI;

    UPROPERTY(meta = (BindWidget))
    class USizeBox* SlotSizeBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmountText;

    UPROPERTY(meta = (BindWidget))
    class UImage* IsEquippedImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* FocusedImage;

    
};
