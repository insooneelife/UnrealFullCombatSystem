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
#include "GameCore/GameUtils.h"


UPickupUI::UPickupUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer), 
    TakeAllKey(EKeys::SpaceBar)
{
    static TSubclassOf<UPickupItemUI> LoadedClass =
        GameUtils::LoadAssetClass<UPickupItemUI>("/Game/DynamicCombatSystem/Widgets/PickupItemWB");

    PickupItemUIClass = LoadedClass;
}

void UPickupUI::NativeConstruct()
{
    Super::NativeConstruct();
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

    TakeAllButton->OnClicked.AddDynamic(this, &UPickupUI::OnClicked_TakeAllButton);
    CloseButton->OnClicked.AddDynamic(this, &UPickupUI::OnClicked_CloseButton);
}

void UPickupUI::NativeDestruct()
{
    TakeAllButton->OnClicked.RemoveDynamic(this, &UPickupUI::OnClicked_TakeAllButton);
    CloseButton->OnClicked.RemoveDynamic(this, &UPickupUI::OnClicked_CloseButton);

    Super::NativeDestruct();
}

FReply UPickupUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    Super::NativeOnKeyDown(InGeometry, InKeyEvent);

    FKey EventKey = UKismetInputLibrary::GetKey(InKeyEvent);

    if (EventKey == BackKey)
    {
        Close();
        return FReply::Handled();
    }
    else
    {
        if (EventKey == TakeAllKey)
        {
            Pickup->TakeAllItems();
            Close();
            return FReply::Handled();
        }
        else
        {
            return FReply::Unhandled();
        }
    }
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

void UPickupUI::OnClicked_TakeAllButton()
{
    Pickup->TakeAllItems();
    Close();
}

void UPickupUI::OnClicked_CloseButton()
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
