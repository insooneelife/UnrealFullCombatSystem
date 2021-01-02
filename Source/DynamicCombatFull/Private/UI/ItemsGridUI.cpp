// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemsGridUI.h"

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
#include "Components/EquipmentComponent.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/ScrollBox.h"
#include "InventoryItemUI.h"
#include "GameCore/GameUtils.h"

UItemsGridUI::UItemsGridUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer), 
    GridColumns(5), 
    DisplayedType(EItemType::MeleeWeapon)
{
    static TSubclassOf<UInventoryItemUI> LoadedClass =
        GameUtils::LoadAssetClass<UInventoryItemUI>("/Game/DynamicCombatSystem/Widgets/InventoryItemWB");

    InventoryItemUIClass = LoadedClass;
}


void UItemsGridUI::NativeConstruct()
{
    Super::NativeConstruct();

    InventoryComponent =
        Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));

    EquipmentComponent =
        Cast<UEquipmentComponent>(GetOwningPlayerPawn()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (GameUtils::IsValid(InventoryComponent))
    {
        InventoryComponent->OnItemAdded.AddDynamic(this, &UItemsGridUI::OnItemAdded);
        InventoryComponent->OnItemRemoved.AddDynamic(this, &UItemsGridUI::OnItemRemoved);
    }
}

void UItemsGridUI::NativeDestruct()
{
    if (GameUtils::IsValid(InventoryComponent))
    {
        InventoryComponent->OnItemAdded.RemoveDynamic(this, &UItemsGridUI::OnItemAdded);
        InventoryComponent->OnItemRemoved.RemoveDynamic(this, &UItemsGridUI::OnItemRemoved);
    }

    Super::NativeDestruct();
}

void UItemsGridUI::InventoryItemClicked(UInventoryItemUI* InItem)
{
    OnInventoryItemClicked.Broadcast(InItem);
}

void UItemsGridUI::CreateItemWidgets()
{
    int NeededSlots = InventoryComponent->GetInventory().Num() - ItemWidgets.Num();

    if (NeededSlots > 0)
    {
        for (int i = 0; i < NeededSlots; ++i)
        {
            UInventoryItemUI* CreatedUI = Cast<UInventoryItemUI> (CreateWidget(GetOwningPlayer(), InventoryItemUIClass));
            CreatedUI->Init(InventoryComponent, EquipmentComponent, this);
            ItemWidgets.Add(CreatedUI);
        }
    }
}

void UItemsGridUI::UpdateItemWidgets(EItemType InType)
{
    DisplayedType = InType;
    ItemsGrid->ClearChildren();

    const TArray<FStoredItem>& Inventory = InventoryComponent->GetInventory();

    int ValidSlots = 0;
    for (const FStoredItem& Item : Inventory)
    {
        FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(Item);

        if ((DefaultItem.Type == InType || InType == EItemType::None) && Item.Amount > 0)
        {
            ItemWidgets[ValidSlots]->UpdateWidget(Item);
            UUniformGridSlot* UniformGrid = ItemsGrid->AddChildToUniformGrid(ItemWidgets[ValidSlots]);
            UniformGrid->SetColumn(ValidSlots % GridColumns);
            UniformGrid->SetRow(ValidSlots / GridColumns);

            ValidSlots++;
        }
    }

    if (ItemWidgets.Num() - ValidSlots > 0)
    {
        for(int i = ValidSlots; i < ItemWidgets.Num(); ++i)
        {
            ItemWidgets[i]->UpdateWidget(FStoredItem());
        }
    }

}

void UItemsGridUI::OnItemAdded(FStoredItem InItem)
{
    if (!IsOpen())
    {
        return;
    }

    UInventoryItemUI* TargetItemUI = nullptr;

    for (UInventoryItemUI* ItemUI : ItemWidgets)
    {
        if (ItemUI->GetItem().Id == InItem.Id)
        {
            TargetItemUI = ItemUI;
        }
    }

    FItem DefaultItem = GameUtils::GetDefaultItemFromStoredItem(InItem);
    if (DefaultItem.Type == DisplayedType)
    {
        if (GameUtils::IsValid(TargetItemUI))
        {
            TargetItemUI->UpdateWidget(InItem);
        }
        else
        {
            CreateItemWidgets();

            int AddedGridIndex = ItemsGrid->GetChildrenCount();
            ItemWidgets[AddedGridIndex]->UpdateWidget(InItem);

            UUniformGridSlot* UniformGrid = ItemsGrid->AddChildToUniformGrid(ItemWidgets[AddedGridIndex]);
            UniformGrid->SetColumn(AddedGridIndex / GridColumns);
            UniformGrid->SetRow(AddedGridIndex / GridColumns);

        }
    }
}

void UItemsGridUI::OnItemRemoved(FStoredItem InItem)
{
    bool bIsOpen = IsOpen();

    // #fix ui is not opened but bIsOpen is true
    if (!bIsOpen)
    {
        return;
    }

    // find target ui
    UInventoryItemUI* TargetItemUI = nullptr;
    for (UInventoryItemUI* ItemUI : ItemWidgets)
    {
        if (ItemUI->GetItem().Id == InItem.Id)
        {
            TargetItemUI = ItemUI;
        }
    }

    if (TargetItemUI != nullptr)
    {
        TargetItemUI->UpdateWidget(InItem);

        if (TargetItemUI->GetItem().Amount <= 0)
        {
            int RemovedGridIndex = ItemsGrid->GetChildIndex(TargetItemUI);
            if (RemovedGridIndex >= 0)
            {
                int ChildCnt = ItemsGrid->GetChildrenCount();

                if (ChildCnt > RemovedGridIndex + 1)
                {
                    for (int i = RemovedGridIndex + 1; i < ChildCnt; ++i)
                    {
                        UUniformGridSlot* UniformGridUI =
                            UWidgetLayoutLibrary::SlotAsUniformGridSlot(ItemsGrid->GetChildAt(i));

                        if (UniformGridUI->Column - 1 < 0)
                        {
                            UniformGridUI->SetColumn(GridColumns - 1);
                            UniformGridUI->SetRow(UniformGridUI->Row - 1);
                        }
                        else
                        {
                            UniformGridUI->SetColumn(UniformGridUI->Column - 1);
                        }
                    }
                }
            }
        }
    }
}

bool UItemsGridUI::IsOpen() const
{
    return IsVisible();
}
