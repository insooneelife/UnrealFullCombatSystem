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

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    void OnClicked();
    void OnHovered();
    void OnUnhovered();

    void OnItemInSlotChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex);
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex);

public:
    void Init(
        UInventoryComponent* InInventoryComponent,
        UEquipmentComponent* InEquipmentComponent,
        UItemsGridUI* InItemsGridUI);

    void UpdateWidget(FStoredItem InItem);
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
