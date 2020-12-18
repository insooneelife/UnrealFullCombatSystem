// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemUI.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "PickupUI.h"
#include "GamePlay/PickupActor.h"
#include "GameCore/GameUtils.h"

void UPickupItemUI::NativeConstruct()
{
    UpdateWidget();
    ItemButton->OnClicked.AddDynamic(this, &UPickupItemUI::OnClickedItemButton);
}

void UPickupItemUI::OnClickedItemButton()
{
    PickupUI->PickupItemClicked(this);
}

void UPickupItemUI::UpdateWidget()
{
    if (UKismetSystemLibrary::IsValidClass(ItemClass))
    {
        FItem DefaultItem = GameUtils::GetDefaultItemFromItemClass(ItemClass);
        ItemImage->SetBrushFromTexture(DefaultItem.Image);

        NameText->SetText(UKismetTextLibrary::Conv_NameToText(DefaultItem.Name));

        if (ItemAmount > 1)
        {
            AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
            AmountText->SetText(UKismetTextLibrary::Conv_IntToText(ItemAmount));
        }
        else
        {
            AmountText->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UPickupItemUI::Init(
    UPickupUI* InPickupUI, TSubclassOf<UItemBase> InItemClass, int InItemAmount, APickupActor* InPickup)
{
    PickupUI = InPickupUI;
    ItemClass = InItemClass;
    ItemAmount = InItemAmount;
    Pickup = InPickup;
}

