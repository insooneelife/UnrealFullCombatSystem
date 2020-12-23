// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEquipmentSlotUI.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GameCore/GameUtils.h"



UActiveEquipmentSlotUI::UActiveEquipmentSlotUI(const FObjectInitializer& ObjectInitializer)
    : 
    Super(ObjectInitializer),
    SlotSize(128.0f, 128.0f)
{
    static UTexture2D* LoadedObject =
        GameUtils::LoadAssetObject<UTexture2D>(TEXT("/Game/DynamicCombatSystem/Widgets/Textures/T_MeleeWeapon"));

    BackgroundTexture = LoadedObject;
}

void UActiveEquipmentSlotUI::NativePreConstruct()
{
    Super::NativePreConstruct();

    SlotSizeBox->SetWidthOverride(SlotSize.X);
    SlotSizeBox->SetHeightOverride(SlotSize.Y);
    BackgroundImage->SetBrushFromTexture(BackgroundTexture);
}

void UActiveEquipmentSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    InventoryComponent = 
        Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));

    EquipmentComponent =
        Cast<UEquipmentComponent>(GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (EquipmentComponent->IsValidLowLevel())
    {
        FStoredItem ActiveItem = EquipmentComponent->GetActiveItem(SlotType, 0);
        UpdateWidget(ActiveItem);
        SetIsHidden(EquipmentComponent->IsSlotHidden(SlotType, 0));

        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UActiveEquipmentSlotUI::OnActiveItemChanged);
        EquipmentComponent->OnSlotHiddenChanged.AddDynamic(this, &UActiveEquipmentSlotUI::OnSlotHiddenChanged);
    }

}

void UActiveEquipmentSlotUI::OnActiveItemChanged(
    FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex)
{
    if (InType == SlotType)
    {
        UpdateWidget(InNewItem);
    }
}

void UActiveEquipmentSlotUI::OnSlotHiddenChanged(
    EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden)
{
    if (InSlotType == SlotType)
    {
        SetIsHidden(bInIsHidden);
    }
}

void UActiveEquipmentSlotUI::UpdateWidget(FStoredItem InItem)
{
    Item = InItem;
    UpdateItemImage();
    UpdateAmountText();
    PlayBlinkBorderAnimation();
}

void UActiveEquipmentSlotUI::UpdateAmountText()
{
    if (Item.ItemClass->IsValidLowLevel())
    {
        UItemBase* DefaultObject = Cast<UItemBase>(Item.ItemClass->GetDefaultObject());

        if (DefaultObject->GetItem().bIsStackable && Item.Amount > 1)
        {
            AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
            AmountText->SetText(UKismetTextLibrary::Conv_IntToText(Item.Amount));            
        }
        else
        {
            AmountText->SetVisibility(ESlateVisibility::Hidden);
        }
    }


}

void UActiveEquipmentSlotUI::UpdateItemImage()
{
    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
    {
        ItemImage->SetVisibility(ESlateVisibility::HitTestInvisible);

        UItemBase* DefaultItem = Cast<UItemBase>(Item.ItemClass->GetDefaultObject());
        ItemImage->SetBrushFromTexture(DefaultItem->GetItem().Image);
    }
    else
    {
        ItemImage->SetVisibility(ESlateVisibility::Hidden);
        ItemImage->SetBrushFromTexture(nullptr);
    }
}

bool UActiveEquipmentSlotUI::SetIsHidden(bool bIsHidden)
{
    ESlateVisibility ImageVisibility = bIsHidden ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible;

    IsHiddenImage->SetVisibility(ImageVisibility);
    BorderCenter->SetIsEnabled(!bIsHidden);

    return false;
}
