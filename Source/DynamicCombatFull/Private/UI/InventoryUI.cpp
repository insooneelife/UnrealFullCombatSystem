// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"
#include "InventoryItemUI.h"
#include "CategoryButtonUI.h"
#include "InputHelpersUI.h"
#include "ItemsGridUI.h"
#include "ItemActionsUI.h"
#include "GameCore/DefaultGameInstance.h"
#include "Interfaces/CanOpenUI.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

    bIsFocusable = true;

    InventoryComponent =
        Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));

    EquipmentComponent =
        Cast<UEquipmentComponent>(GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    Close();

    InventoryKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::Inventory);
    BackKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::UIBack);

    FText InventoryDisplayName = UKismetInputLibrary::Key_GetDisplayName(InventoryKey);
    InputHelpers->AddInputHelper(InventoryDisplayName, FText::FromString(TEXT("Close")));

    FText BackDisplayName = UKismetInputLibrary::Key_GetDisplayName(BackKey);
    InputHelpers->AddInputHelper(BackDisplayName, FText::FromString(TEXT("Back")));

    CloseButton->OnClicked.AddDynamic(this, &UInventoryUI::OnClicked_CloseButton);
    InventoryItemsGrid->OnInventoryItemClicked.AddDynamic(this, &UInventoryUI::OnInventoryItemClicked);
}

FReply UInventoryUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    FKey EventKey = UKismetInputLibrary::GetKey(InKeyEvent);

    if (EventKey == InventoryKey)
    {
        Close();
        return FReply::Handled();
    }
    else
    {
        if (EventKey == BackKey)
        {
            Close();
            return FReply::Handled();
        }
        else
        {
            return FReply::Unhandled();
        }
    }
}


void UInventoryUI::FocusSelf()
{
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this, EMouseLockMode::LockAlways);
}

void UInventoryUI::OnInventoryItemClicked(UInventoryItemUI* InItem)
{
    FVector2D SpawnPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
    UItemActionsUI* CreatedUI = Cast<UItemActionsUI>(CreateWidget(GetOwningPlayer(), ItemActionsUIClass));
    CreatedUI->Init(InventoryComponent, SpawnPosition, InItem->GetItem());

    CreatedUI->OnWidgetRemoved.AddDynamic(this, &UInventoryUI::FocusSelf);
    CreatedUI->AddToViewport();
}

void UInventoryUI::OnClicked_CloseButton()
{
    Close();
}

void UInventoryUI::Open()
{
    SetVisibility(ESlateVisibility::Visible);
    InventoryItemsGrid->CreateItemWidgets();
    InventoryItemsGrid->UpdateItemWidgets(GetDisplayedType());

    FocusSelf();

    ICanOpenUI* CanOpenUI = Cast<ICanOpenUI>(GetOwningPlayerPawn());

    if (CanOpenUI != nullptr)
    {
        CanOpenUI->OpenedUI();
    }
}

void UInventoryUI::Close()
{
    SetVisibility(ESlateVisibility::Hidden);
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());

    ICanOpenUI* CanOpenUI = Cast<ICanOpenUI>(GetOwningPlayerPawn());
    if (CanOpenUI != nullptr)
    {
        CanOpenUI->ClosedUI();
    }
}


void UInventoryUI::CategoryButtonClicked(UCategoryButtonUI* Button)
{
    if (Button != SelectedCategoryButton)
    {
        SetCategoryButton(Button);
        InventoryItemsGrid->CreateItemWidgets();
        InventoryItemsGrid->UpdateItemWidgets(GetDisplayedType());
    }
}

void UInventoryUI::SetCategoryButton(UCategoryButtonUI* Button)
{
    if (Button != SelectedCategoryButton)
    {
        if (SelectedCategoryButton->IsValidLowLevel())
        {
            SelectedCategoryButton->SetActiveBorder(false);
            if (Button->IsValidLowLevel())
            {
                SelectedCategoryButton = Button;
                SelectedCategoryButton->SetActiveBorder(true);
            }
        }
    }
}

EItemType UInventoryUI::GetDisplayedType() const
{
    return UKismetSystemLibrary::IsValid(SelectedCategoryButton) ? SelectedCategoryButton->GetItemType() : EItemType::None;
}
