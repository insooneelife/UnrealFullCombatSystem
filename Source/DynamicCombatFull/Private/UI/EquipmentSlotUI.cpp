// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"

#include "Components/EquipmentComponent.h"
#include "EquipmentUI.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GameCore/GameUtils.h"


void UEquipmentSlotUI::NativePreConstruct()
{
    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
    BackgroundImage->SetBrushFromTexture(BackgroundTexture);
}

void UEquipmentSlotUI::NativeConstruct()
{
    EquipmentComponent = Cast<UEquipmentComponent>(
        GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UEquipmentUI::StaticClass(), false);

    EquipmentUI = Cast<UEquipmentUI>(FoundWidgets[0]);

    if (EquipmentComponent->IsValidLowLevel())
    {
        FStoredItem SlotItem = EquipmentComponent->GetItemInSlot(ItemType, SlotIndex, ItemIndex);
        UpdateWidget(SlotItem);

        bool bVisible = EquipmentComponent->IsActiveItemIndex(ItemType, SlotIndex, ItemIndex);
        SetActiveWidget(bVisible);

        EquipmentComponent->OnItemInSlotChanged.AddDynamic(this, &UEquipmentSlotUI::OnItemInSlotChanged);
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UEquipmentSlotUI::OnActiveItemChanged);
    }

    SlotButton->OnClicked.AddDynamic(this, &UEquipmentSlotUI::OnClickedSlotButton);
    SlotButton->OnHovered.AddDynamic(this, &UEquipmentSlotUI::OnHoveredSlotButton);
    SlotButton->OnUnhovered.AddDynamic(this, &UEquipmentSlotUI::OnUnhoveredSlotButton);
}

void UEquipmentSlotUI::OnClickedSlotButton()
{
    EquipmentUI->EquipmentSlotClicked(this);
}

void UEquipmentSlotUI::OnHoveredSlotButton()
{
    BackgroundImage->SetColorAndOpacity(GameUtils::Gray);
}

void UEquipmentSlotUI::OnUnhoveredSlotButton()
{
    BackgroundImage->SetColorAndOpacity(GameUtils::Black);
}

void UEquipmentSlotUI::UpdateWidget(FStoredItem InItem)
{
    Item = InItem;
    UpdateImage();
    UpdateAmountText();
}

void UEquipmentSlotUI::UpdateAmountText()
{
    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
    {
        FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);

        if (DefaultItem.bIsStackable && Item.Amount > 1)
        {
            AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);

            FText InText = UKismetTextLibrary::Conv_IntToText(Item.Amount);
            AmountText->SetText(InText);
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

void UEquipmentSlotUI::UpdateImage()
{
    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount >= 1)
    {
        FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);

        ItemImage->SetBrushFromTexture(DefaultItem.Image);
        ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        ItemImage->SetBrushFromTexture(nullptr);
        ItemImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UEquipmentSlotUI::SetActiveWidget(bool bVisible)
{
    FLinearColor Color = bShowActiveBorder ? GameUtils::Brown : GameUtils::Gray;

    ActiveBorder->SetBrushColor(Color);
}

void UEquipmentSlotUI::OnItemInSlotChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex)
{
    if (ItemType == InType && SlotIndex == InSlotIndex && ItemIndex == InItemIndex)
    {
        UpdateWidget(InNewItem);
    }
}

void UEquipmentSlotUI::OnActiveItemChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex)
{
    if (InType == ItemType && SlotIndex == InSlotIndex)
    {
        SetActiveWidget(ItemIndex == InItemIndex);
    }
}
