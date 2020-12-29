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

UEquipmentSlotUI::UEquipmentSlotUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer), 
    SlotSize(96.0f), 
    bShowActiveBorder(true)
{
    static UTexture2D* LoadedObject =
        GameUtils::LoadAssetObject<UTexture2D>(TEXT("/Game/DynamicCombatSystem/Widgets/Textures/T_MeleeWeapon"));

    BackgroundTexture = LoadedObject;
}

void UEquipmentSlotUI::NativePreConstruct()
{
    Super::NativePreConstruct();

    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
    BackgroundImage->SetBrushFromTexture(BackgroundTexture);
}

void UEquipmentSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    EquipmentComponent = Cast<UEquipmentComponent>(
        GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (EquipmentComponent->IsValidLowLevel())
    {
        FStoredItem SlotItem = EquipmentComponent->GetItemInSlot(ItemType, SlotIndex, ItemIndex);
        UpdateWidget(SlotItem);

        bool bVisible = EquipmentComponent->IsActiveItemIndex(ItemType, SlotIndex, ItemIndex);
        SetActiveWidget(bVisible);

        EquipmentComponent->OnItemInSlotChanged.AddDynamic(this, &UEquipmentSlotUI::OnItemInSlotChanged);
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UEquipmentSlotUI::OnActiveItemChanged);
    }

    SlotButton->OnClicked.AddDynamic(this, &UEquipmentSlotUI::OnClicked_SlotButton);
    SlotButton->OnHovered.AddDynamic(this, &UEquipmentSlotUI::OnHovered_SlotButton);
    SlotButton->OnUnhovered.AddDynamic(this, &UEquipmentSlotUI::OnUnhovered_SlotButton);

    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, EquipmentUIClass, false);

    if (FoundWidgets.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("FoundWidgets is empty!!  %s"), *this->GetName());
        return;
    }
    else
    {
        EquipmentUI = Cast<UEquipmentUI>(FoundWidgets[0]);
    }

}

void UEquipmentSlotUI::NativeDestruct()
{
    if (EquipmentComponent->IsValidLowLevel())
    {
        EquipmentComponent->OnItemInSlotChanged.RemoveDynamic(this, &UEquipmentSlotUI::OnItemInSlotChanged);
        EquipmentComponent->OnActiveItemChanged.RemoveDynamic(this, &UEquipmentSlotUI::OnActiveItemChanged);
    }

    SlotButton->OnClicked.RemoveDynamic(this, &UEquipmentSlotUI::OnClicked_SlotButton);
    SlotButton->OnHovered.RemoveDynamic(this, &UEquipmentSlotUI::OnHovered_SlotButton);
    SlotButton->OnUnhovered.RemoveDynamic(this, &UEquipmentSlotUI::OnUnhovered_SlotButton);

    Super::NativeDestruct();
}

void UEquipmentSlotUI::OnClicked_SlotButton()
{
    EquipmentUI->EquipmentSlotClicked(this);
}

void UEquipmentSlotUI::OnHovered_SlotButton()
{
    BackgroundImage->SetColorAndOpacity(GameUtils::Gray);
}

void UEquipmentSlotUI::OnUnhovered_SlotButton()
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
