// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotOnlyDisplayUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/EquipmentComponent.h"
#include "EquipmentUI.h"
#include "GameCore/GameUtils.h"

void UEquipmentSlotOnlyDisplayUI::NativePreConstruct()
{
    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
    BackgroundImage->SetBrushFromTexture(BackgroundTexture);
}

void UEquipmentSlotOnlyDisplayUI::NativeConstruct()
{
    EquipmentComponent = 
        Cast<UEquipmentComponent>(GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UEquipmentUI::StaticClass());

    EquipmentUI = Cast<UEquipmentUI>(FoundWidgets[0]);

    if (EquipmentUI->IsValidLowLevel())
    {
        UpdateWidget(EquipmentComponent->GetItemInSlot(ItemType, SlotIndex, ItemIndex));
        SetActiveWidget(EquipmentComponent->IsActiveItemIndex(ItemType, SlotIndex, ItemIndex));
    }

    if (EquipmentComponent->IsValidLowLevel())
    {
        EquipmentComponent->OnItemInSlotChanged.AddDynamic(this, &UEquipmentSlotOnlyDisplayUI::OnItemInSlotChanged);
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UEquipmentSlotOnlyDisplayUI::OnActiveItemChanged);
    }


}

void UEquipmentSlotOnlyDisplayUI::OnItemInSlotChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex)
{
    if (InType == ItemType && InSlotIndex == SlotIndex && ItemIndex == InItemIndex)
    {
        UpdateWidget(InNewItem);
    }
}

void UEquipmentSlotOnlyDisplayUI::OnActiveItemChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex)
{
    if (InType == ItemType && InSlotIndex == SlotIndex)
    {
        SetActiveWidget(ItemIndex == InItemIndex);
    }
}


void UEquipmentSlotOnlyDisplayUI::UpdateWidget(FStoredItem InItem)
{
    Item = InItem;
    UpdateImage();
    UpdateAmountText();
}

void UEquipmentSlotOnlyDisplayUI::UpdateAmountText()
{
    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
    {
        FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);

        if (DefaultItem.bIsStackable && Item.Amount > 1)
        {
            AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
            FText Text = UKismetTextLibrary::Conv_IntToText(Item.Amount);
            AmountText->SetText(Text);
        }
        else
        {
            AmountText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else
    {
        AmountText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UEquipmentSlotOnlyDisplayUI::UpdateImage()
{
    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount > 1)
    {
        FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);
        ItemImage->SetBrushFromTexture(DefaultItem.Image);
        AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        ItemImage->SetBrushFromTexture(nullptr);
        AmountText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UEquipmentSlotOnlyDisplayUI::SetActiveWidget(bool bInVisible)
{
    if (bShowActiveBorder)
    {
        FLinearColor Color = bInVisible ? GameUtils::Brown : GameUtils::Gray;
        ActiveBorder->SetBrushColor(Color);
    }
}
