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

UInventoryItemUI::UInventoryItemUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer), SlotSize(96.0f)
{
}

void UInventoryItemUI::NativePreConstruct()
{
    Super::NativePreConstruct();
    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
}

void UInventoryItemUI::NativeConstruct()
{
    Super::NativeConstruct();

    SetFocusedImage(false);
    if (EquipmentComponent->IsValidLowLevel())
    {
        EquipmentComponent->OnItemInSlotChanged.AddDynamic(this, &UInventoryItemUI::OnItemInSlotChanged);
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UInventoryItemUI::OnActiveItemChanged);
    }

    SlotButton->OnClicked.AddDynamic(this, &UInventoryItemUI::OnClicked_SlotButton);
    SlotButton->OnHovered.AddDynamic(this, &UInventoryItemUI::OnHovered_SlotButton);
    SlotButton->OnUnhovered.AddDynamic(this, &UInventoryItemUI::OnUnhovered_SlotButton);
}

void UInventoryItemUI::NativeDestruct()
{
    EquipmentComponent->OnItemInSlotChanged.RemoveDynamic(this, &UInventoryItemUI::OnItemInSlotChanged);
    EquipmentComponent->OnActiveItemChanged.RemoveDynamic(this, &UInventoryItemUI::OnActiveItemChanged);

    SlotButton->OnClicked.RemoveDynamic(this, &UInventoryItemUI::OnClicked_SlotButton);
    SlotButton->OnHovered.RemoveDynamic(this, &UInventoryItemUI::OnHovered_SlotButton);
    SlotButton->OnUnhovered.RemoveDynamic(this, &UInventoryItemUI::OnUnhovered_SlotButton);

    Super::NativeDestruct();
}

void UInventoryItemUI::OnClicked_SlotButton()
{
    ItemsGridUI->InventoryItemClicked(this);
}


void UInventoryItemUI::OnHovered_SlotButton()
{
    SetFocusedImage(true);
}

void UInventoryItemUI::OnUnhovered_SlotButton()
{
    SetFocusedImage(false);
}

void UInventoryItemUI::OnItemInSlotChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex)
{
    if (IsOpen())
    {
        if (OldItem.Id == Item.Id || NewItem.Id == Item.Id)
        {
            UpdateIsEquippedImage();
        }
    }
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
    ESlateVisibility FocusedImageVisibility = bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
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
