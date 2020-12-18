// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemUI.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"

#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "ItemsGridUI.h"
#include "GameCore/GameUtils.h"

void UInventoryItemUI::NativePreConstruct()
{
    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
}

void UInventoryItemUI::NativeConstruct()
{
    SetFocusedImage(false);
    if (EquipmentComponent->IsValidLowLevel())
    {
        EquipmentComponent->OnItemInSlotChanged.AddDynamic(this, &UInventoryItemUI::OnItemInSlotChanged);
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UInventoryItemUI::OnActiveItemChanged);
    }

    SlotButton->OnClicked.AddDynamic(this, &UInventoryItemUI::OnClicked);
    SlotButton->OnHovered.AddDynamic(this, &UInventoryItemUI::OnHovered);
    SlotButton->OnUnhovered.AddDynamic(this, &UInventoryItemUI::OnUnhovered);
}

void UInventoryItemUI::OnClicked()
{
    ItemsGridUI->InventoryItemClicked(this);
}

void UInventoryItemUI::OnItemInSlotChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex)
{
    if (IsOpen())
    {
        if (OldItem.Id == Item.Id || NewItem.Id ==Item.Id)
        {
            UpdateIsEquippedImage();
        }
    }
}

void UInventoryItemUI::OnHovered()
{
    SetFocusedImage(true);
}

void UInventoryItemUI::OnUnhovered()
{
    SetFocusedImage(false);
}

void UInventoryItemUI::OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex)
{
    if (IsOpen())
    {
        if (OldItem.Id == Item.Id || NewItem.Id == Item.Id)
        {
            UpdateIsEquippedImage();
        }
    }
}

void UInventoryItemUI::Init(
    UInventoryComponent* InInventoryComponent,
    UEquipmentComponent* InEquipmentComponent,
    UItemsGridUI* InItemsGridUI)
{
    InventoryComponent = InInventoryComponent;
    EquipmentComponent = InEquipmentComponent;
    ItemsGridUI = InItemsGridUI;
}

void UInventoryItemUI::UpdateWidget(FStoredItem InItem)
{
    Item = InItem;
    if (IsOpen())
    {
        if (UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount > 0)
        {
            SetVisibility(ESlateVisibility::Visible);

            FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);
            ItemImage->SetBrushFromTexture(DefaultItem.Image);

            UpdateAmountText();
            UpdateIsEquippedImage();
        }
        else
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UInventoryItemUI::UpdateAmountText()
{
    if (Item.Amount > 0)
    {
        AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
        AmountText->SetText(UKismetTextLibrary::Conv_IntToText(Item.Amount));
    }
    else
    {
        AmountText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInventoryItemUI::UpdateIsEquippedImage()
{
    ESlateVisibility ItemEquippedVisibility =
        EquipmentComponent->IsItemEquipped(Item.Id) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
    IsEquippedImage->SetVisibility(ItemEquippedVisibility);

    FLinearColor Color = EquipmentComponent->IsItemActive(Item.Id) ? GameUtils::Brown : GameUtils::Red;
    IsEquippedImage->SetColorAndOpacity(Color);
}

void UInventoryItemUI::SetFocusedImage(bool bVisible)
{
    ESlateVisibility FocusedImageVisibility = bVisible ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;
    FocusedImage->SetVisibility(FocusedImageVisibility);
}

bool UInventoryItemUI::IsOpen() const
{
    if (ItemsGridUI->IsOpen())
    {
        return true;
    }
    
    return false;
}
