// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentUI.h"
#include "Kismet/KismetInputLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"

#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"

#include "Interfaces/CanOpenUI.h"
#include "GameCore/DefaultGameInstance.h"
#include "InputHelpersUI.h"
#include "ItemsGridUI.h"
#include "InventoryItemUI.h"
#include "EquipmentSlotUI.h"

void UEquipmentUI::NativeConstruct()
{
    Super::NativeConstruct();

    bIsFocusable = true;
    GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass());
    GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass());

    InventoryItemsGrid->OnInventoryItemClicked.AddDynamic(this, &UEquipmentUI::OnInventoryItemClicked);
    CloseButton->OnClicked.AddDynamic(this, &UEquipmentUI::OnClicked_CloseButton);
    Close();

    EquipmentKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::Equipment);
    BackKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::UIBack);

    FText EquipmentDisplayName = UKismetInputLibrary::Key_GetDisplayName(EquipmentKey);
    InputHelpers->AddInputHelper(EquipmentDisplayName, FText::FromString(TEXT("Close")));

    FText BackDisplayName = UKismetInputLibrary::Key_GetDisplayName(BackKey);
    InputHelpers->AddInputHelper(BackDisplayName, FText::FromString("Back"));
}

FReply UEquipmentUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    FKey InKey = UKismetInputLibrary::GetKey(InKeyEvent);

    if (InKey == EquipmentKey)
    {
        Close();
        return FReply::Handled();
    }
    else
    {
        if (InKey == BackKey)
        {
            int Index = EquipmentWidgetSwitcher->GetActiveWidgetIndex();

            if (Index == 0)
            {
                Close();
                return FReply::Handled();
            }
            else if (Index == 1)
            {
                SetActiveWidget(0);
                return FReply::Handled();
            }
            else
            {
                return FReply::Unhandled();
            }
        }
        else
        {
            return FReply::Unhandled();
        }
    }
}

void UEquipmentUI::Open()
{
    SetVisibility(ESlateVisibility::Visible);

    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this, EMouseLockMode::LockAlways);
    SetActiveWidget(0);
    
    ICanOpenUI* CanOpenUI = Cast<ICanOpenUI>(GetOwningPlayerPawn());

    if (CanOpenUI != nullptr)
    {
        CanOpenUI->OpenedUI();
    }
}

void UEquipmentUI::Close()
{
    SetVisibility(ESlateVisibility::Hidden);

    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
    ICanOpenUI* CanOpenUI = Cast<ICanOpenUI>(GetOwningPlayerPawn());
    if (CanOpenUI != nullptr)
    {
        CanOpenUI->ClosedUI();
    }
}

void UEquipmentUI::OnInventoryItemClicked(UInventoryItemUI* InItem)
{
    if (ClickedSlot->IsValidLowLevel())
    {
        EquipmentComponent->UpdateItemInSlot(
            ClickedSlot->GetItemType(),
            ClickedSlot->GetSlotIndex(),
            ClickedSlot->GetItemIndex(),
            ClickedSlot->GetItem(), 
            EHandleSameItemMethod::Unequip);

    }
}

void UEquipmentUI::OnClicked_CloseButton()
{
    Close();
}

void UEquipmentUI::EquipmentSlotClicked(UEquipmentSlotUI* InSlot)
{
    ClickedSlot = InSlot;
    SetActiveWidget(1);
}

void UEquipmentUI::SetActiveWidget(int WidgetIndex)
{
    if (EquipmentWidgetSwitcher->GetActiveWidgetIndex() != WidgetIndex)
    {
        if (WidgetIndex == 0)
        {
            EquipmentWidgetSwitcher->SetActiveWidgetIndex(0);
        }
        else if (WidgetIndex == 1)
        {
            EquipmentWidgetSwitcher->SetActiveWidgetIndex(1);

            InventoryItemsGrid->CreateItemWidgets();
            InventoryItemsGrid->UpdateItemWidgets(ClickedSlot->GetItemType());
        }
    }
}
