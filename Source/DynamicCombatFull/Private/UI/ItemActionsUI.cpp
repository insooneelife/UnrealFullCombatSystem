// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActionsUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetInputLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/InventoryComponent.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"

void UItemActionsUI::NativeConstruct()
{
    Super::NativeConstruct();

    bIsFocusable = true;
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this, EMouseLockMode::LockAlways);
    SetPosition(SpawnPosition);

    FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);

    DefaultItem.bIsDroppable;
    UseButton->SetIsEnabled(DefaultItem.bIsConsumable);
    DropButton->SetIsEnabled(DefaultItem.bIsDroppable);

    BackKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::UIBack);

    UseButton->OnClicked.AddDynamic(this, &UItemActionsUI::OnClicked_UseButton);
    DropButton->OnClicked.AddDynamic(this, &UItemActionsUI::OnClicked_DropButton);
}

void UItemActionsUI::NativeDestruct()
{
    UseButton->OnClicked.RemoveDynamic(this, &UItemActionsUI::OnClicked_UseButton);
    DropButton->OnClicked.RemoveDynamic(this, &UItemActionsUI::OnClicked_DropButton);

    Super::NativeDestruct();
}

void UItemActionsUI::Close()
{
    OnWidgetRemoved.Broadcast();
    RemoveFromParent();
}

void UItemActionsUI::OnClicked_UseButton()
{
    if (GameUtils::IsValid(InventoryComponent))
    {
        InventoryComponent->UseItem(Item.Id);
        Close();
    }
}

void UItemActionsUI::OnClicked_DropButton()
{
    if (GameUtils::IsValid(InventoryComponent))
    {
        InventoryComponent->DropItem(Item);
        Close();
    }
}

FReply UItemActionsUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

    FKey EventKey = UKismetInputLibrary::PointerEvent_GetEffectingButton(InMouseEvent);

    if (ActionsSB->IsHovered() && EventKey == EKeys::LeftMouseButton)
    {
        return FReply::Unhandled();
    }
    else
    {
        Close();
        return FReply::Handled();
    }
}

FReply UItemActionsUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    if (UKismetInputLibrary::GetKey(InKeyEvent) == BackKey)
    {
        Close();
        return FReply::Handled();
    }
    else
    {
        return FReply::Unhandled();
    }
}

void UItemActionsUI::SetPosition(FVector2D Position)
{
    float FinalSpawnPositionX;
    if (Position.X + ActionsSB->WidthOverride > UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X)
    {
        FinalSpawnPositionX = Position.X - ActionsSB->WidthOverride;
    }
    else
    {
        FinalSpawnPositionX = Position.X; 
    }


    float FinalSpawnPositionY;
    if (Position.Y + ActionsSB->HeightOverride > UWidgetLayoutLibrary::GetViewportSize(GetWorld()).Y)
    {
        FinalSpawnPositionY = Position.Y - ActionsSB->HeightOverride;
    }
    else
    {
        FinalSpawnPositionY = Position.Y;
    }

    UWidgetLayoutLibrary::SlotAsCanvasSlot(ActionsSB)->SetPosition(FVector2D(FinalSpawnPositionX, FinalSpawnPositionY));
}

void UItemActionsUI::Init(UInventoryComponent* InInventoryComponent, FVector2D InSpawnPosition, const FStoredItem& InItem)
{
    InventoryComponent = InInventoryComponent;
    SpawnPosition = InSpawnPosition;
    Item = InItem;
}