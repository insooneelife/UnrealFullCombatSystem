// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupUI.h"

#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"

#include "Components/InventoryComponent.h"

#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"
#include "InputHelpersUI.h"
#include "PickupItemUI.h"
#include "GamePlay/PickupActor.h"
#include "Interfaces/CanOpenUI.h"



void UPickupUI::NativeConstruct()
{
    bIsFocusable = true;

    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this, EMouseLockMode::LockAlways);

    ICanOpenUI* CanOpenUI = Cast<ICanOpenUI>(GetOwningPlayerPawn());
    if (CanOpenUI != nullptr)
    {
        CanOpenUI->OpenedUI();
    }
   
    InventoryComponent = 
        Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));

    BackKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::UIBack);
    TakeAllKey = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::UITakeAll);

    FText TakeAllKeyName = UKismetInputLibrary::Key_GetDisplayName(TakeAllKey);
    InputHelpers->AddInputHelper(TakeAllKeyName, FText::FromName(UDefaultGameInstance::TakeAll));

    FText BackKeyName = UKismetInputLibrary::Key_GetDisplayName(BackKey);
    InputHelpers->AddInputHelper(BackKeyName, FText::FromName(UDefaultGameInstance::Back));

    FText FinalText = UKismetTextLibrary::Conv_NameToText(Pickup->GetName());
    CreateItemWidgets();
    PickupNameText->SetText(FinalText);

    TakeAllButton->OnClicked.AddDynamic(this, &UPickupUI::OnClickedTakeAllButton);
    CloseButton->OnClicked.AddDynamic(this, &UPickupUI::OnClickedCloseButton);
}

FReply UPickupUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UPickupUI::Close()
{
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());

    ICanOpenUI* CanOpenUI = Cast<ICanOpenUI>(GetOwningPlayerPawn());
    if (CanOpenUI != nullptr)
    {
        CanOpenUI->ClosedUI();
    }

    RemoveFromParent();
}

void UPickupUI::OnClickedTakeAllButton()
{
    Pickup->TakeAllItems();
    Close();
}

void UPickupUI::OnClickedCloseButton()
{
    Close();
}

void UPickupUI::CreateItemWidgets()
{
    PickupScrollBox->ClearChildren();
    const TMap<TSubclassOf<UItemBase>, int>& Items = Pickup->GetItems();

    for (auto E : Items)
    {
        TSubclassOf<UItemBase> ItemClass = E.Key;
        int ItemAmount = E.Value;

        UPickupItemUI* CreatedUI = Cast<UPickupItemUI> (CreateWidget(GetOwningPlayer(), PickupItemUIClass));
        CreatedUI->Init(this, ItemClass, ItemAmount, Pickup);

        PickupScrollBox->AddChild(CreatedUI);
        CreatedUI->SetPadding(FMargin(2.0f, 2.0f, 2.0f, 2.0f));
    }
}

void UPickupUI::PickupItemClicked(UPickupItemUI* Item)
{
    Pickup->TakeItem(Item->GetItemClass());

    PickupScrollBox->RemoveChild(Item);

    if (PickupScrollBox->HasAnyChildren())
    {
        Close();
    }
}
