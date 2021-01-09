// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "Interfaces/ItemIsDisplayed.h"
#include "Interfaces/ItemCanBlock.h"
#include "Interfaces/ItemCanBeTwoHanded.h"
#include "GamePlay/DCSSaveGame.h"
#include "GamePlay/Items/ObjectItems/WeaponItem.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
:
    EquipmentSlots(
{
    FEquipmentSlots(EItemType::Spell, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Shield, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Head, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Top, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Legs, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Hands, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Feet, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Arrows, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem(), FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Tool, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem(), FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Ring, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false),
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false),
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false),
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::MeleeWeapon, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem(), FStoredItem(), FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::RangeWeapon, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem(), FStoredItem(), FStoredItem()}, 0, false)
    }),
    FEquipmentSlots(EItemType::Necklace, TArray<FEquipmentSlot> {
        FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
    }),
})
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    
    DisplayedItems = { {EItemType::Ring, FDisplayedItems(TArray<ADisplayedItem*>{nullptr, nullptr, nullptr})} };

    MainHandTypes = { EItemType::MeleeWeapon, EItemType::Spell, EItemType::RangeWeapon };

    SelectedMainHandType = EItemType::MeleeWeapon;
}

void UEquipmentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto E : DisplayedItems)
    {
        const FDisplayedItems& Item = E.Value;

        for (ADisplayedItem* DisplayedItem : Item.DisplayedItems)
        {
            if (DisplayedItem != nullptr)
            {
                DisplayedItem->Destroy();
            }
        }
    }

    DisplayedItems.Empty();

    if (Inventory != nullptr)
    {
        Inventory->OnItemRemoved.RemoveDynamic(this, &UEquipmentComponent::OnItemModified);
        Inventory->OnItemAdded.RemoveDynamic(this, &UEquipmentComponent::OnItemModified);
    }

    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (GameUtils::IsValid(GameMode))
        {
            GameMode->OnGameLoaded.RemoveDynamic(this, &UEquipmentComponent::OnGameLoaded);
        }
    }

    Super::EndPlay(EndPlayReason);
}

ADisplayedItem* UEquipmentComponent::GetDisplayedItem(EItemType InType, int InSlotIndex) const
{
    if (DisplayedItems.Contains(InType))
    {
        return DisplayedItems[InType].DisplayedItems[InSlotIndex];
    }
    return nullptr;
}

FStoredItem UEquipmentComponent::GetActiveItem(EItemType InType, int InSlotIndex) const
{
    int Index = GetEqSlotsIndex(InType);
    const FEquipmentSlot& Slot = EquipmentSlots[Index].Slots[InSlotIndex];
    return Slot.Items[Slot.ActiveItemIndex];
}


bool UEquipmentComponent::IsSlotHidden(EItemType InType, int InSlotIndex) const
{
    if (IsSlotIndexValid(InType, InSlotIndex))
    {
        int Index = GetEqSlotsIndex(InType);
        const FEquipmentSlot& Slot = EquipmentSlots[Index].Slots[InSlotIndex];
        return Slot.bIsHidden;
    }

    return false;
}

bool UEquipmentComponent::IsShieldEquipped() const
{
    if (IsSlotHidden(EItemType::Shield, 0))
    {
        return false;
    }
    else
    {
        FStoredItem Item = GetActiveItem(EItemType::Shield, 0);

        if (IsItemValid(Item))
        {
            // ???
            IItemCanBlock* ItemCanBlock = Cast<IItemCanBlock>(NewObject<UItemBase>(GetOwner(), Item.ItemClass));
            return ItemCanBlock != nullptr;
        }
        else
        {
            return false;
        }
    }
}

bool UEquipmentComponent::IsInCombat() const
{
    return bIsInCombat;
}

void UEquipmentComponent::ToggleCombat()
{
    SetCombat(!bIsInCombat);
}

bool UEquipmentComponent::IsTwoHandedWeaponEquipped() const
{
    if (IsItemTwoHanded(GetWeapon()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void UEquipmentComponent::Init()
{
    // inventory events
    Inventory = Cast <UInventoryComponent> (GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));

    if (Inventory != nullptr)
    {
        Inventory->OnItemRemoved.AddDynamic(this, &UEquipmentComponent::OnItemModified);
        Inventory->OnItemAdded.AddDynamic(this, &UEquipmentComponent::OnItemModified);
    }
   
    // build equipment
    BuildEquipment(EquipmentSlots);

    // if owner is a player, rebuild equipment on game load
    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (GameUtils::IsValid(GameMode))
        {
            GameMode->OnGameLoaded.AddDynamic(this, &UEquipmentComponent::OnGameLoaded);
        }        
    }
}

void UEquipmentComponent::SwitchMainHandType(bool bInForward)
{
    int SelectedIndex = MainHandTypes.Find(SelectedMainHandType);

    if (SelectedIndex >= 0)
    {
        int NextIndex = GetNextArrayIndex(MainHandTypes, SelectedIndex, bInForward);

        SetMainHandType(MainHandTypes[NextIndex]);
    }
}

void UEquipmentComponent::SwitchSlotActiveIndex(
    EItemType InType, int InSlotIndex, bool bInForward, bool bInIgnoreEmptyItems)
{
    if (IsSlotIndexValid(InType, InSlotIndex))
    {
        int ActiveItemIndex = GetActiveItemIndex(InType, InSlotIndex);

        int EqSlotsIndex = GetEqSlotsIndex(InType);

        FEquipmentSlot& EqSlot = EquipmentSlots[EqSlotsIndex].Slots[InSlotIndex];
        const TArray<FStoredItem>& Items = EqSlot.Items;

        int NewIndex = GetNextArrayIndex(Items, ActiveItemIndex, bInForward);

        if (bInIgnoreEmptyItems)
        {
            for (int i = 1; i < Items.Num(); ++i)
            {
                if (IsItemValid(Items[NewIndex]))
                {
                    EqSlot.ActiveItemIndex = NewIndex;
                    ActiveItemChanged(EqSlot.Items[ActiveItemIndex], EqSlot.Items[NewIndex], InType, InSlotIndex, NewIndex);
                }
                else
                {
                    NewIndex = GetNextArrayIndex(Items, NewIndex, bInForward);
                }
            }
        }
        else
        {
            EqSlot.ActiveItemIndex = NewIndex;
            ActiveItemChanged(EqSlot.Items[ActiveItemIndex], EqSlot.Items[NewIndex], InType, InSlotIndex, NewIndex);
        }
    }
}

void UEquipmentComponent::SetMainHandType(EItemType InType)
{
    if (MainHandTypes.Find(InType) >= 0)
    {
        if (SelectedMainHandType != InType)
        {
            EItemType PreviousType = SelectedMainHandType;
            SelectedMainHandType = InType;

            UpdateCombatType();

            SetSlotHidden(PreviousType, 0, true);
            SetSlotHidden(SelectedMainHandType, 0, false);

            // Update shield if new weapon is Two Handed
            FStoredItem Item = GetItemInSlot(SelectedMainHandType, 0, GetActiveItemIndex(SelectedMainHandType, 0));
            if (IsItemTwoHanded(Item))
            {
                SetSlotHidden(EItemType::Shield, 0, true);
            }
            else
            {
                SetSlotHidden(EItemType::Shield, 0, false);
            }


            SetSlotHidden(
                EItemType::Arrows,
                0,
                !(SelectedMainHandType == EItemType::RangeWeapon && GetCombatType() == ECombatType::Range));

            OnMainHandTypeChanged.Broadcast(SelectedMainHandType);
        }
    }
}

void UEquipmentComponent::SetSlotActiveIndex(EItemType InType, int InSlotIndex, int InNewActiveIndex)
{
    if (IsItemIndexValid(InType, InSlotIndex, InNewActiveIndex))
    {
        int ActiveItemIndex = GetActiveItemIndex(InType, InSlotIndex);

        int EqSlotIndex = GetEqSlotsIndex(InType);

        if (ActiveItemIndex != InNewActiveIndex &&
            ActiveItemIndex >= 0 &&
            InNewActiveIndex >= 0)
        {
            FEquipmentSlot& EqSlot = EquipmentSlots[EqSlotIndex].Slots[InSlotIndex];
            EqSlot.ActiveItemIndex = InNewActiveIndex;
            ActiveItemChanged(
                EqSlot.Items[ActiveItemIndex],
                EqSlot.Items[InNewActiveIndex], InType, InSlotIndex, InNewActiveIndex);
        }
    }
}

FStoredItem UEquipmentComponent::GetItemInSlot(EItemType InType, int InSlotIndex, int InItemIndex) const
{
    if (IsItemIndexValid(InType, InSlotIndex, InItemIndex))
    {
        return EquipmentSlots[GetEqSlotsIndex(InType)].Slots[InSlotIndex].Items[InItemIndex];
    }
    else
    {
        return FStoredItem();
    }
}

void UEquipmentComponent::UpdateItemInSlot(
    EItemType InType, 
    int InSlotIndex, 
    int InItemIndex, 
    FStoredItem InItem, 
    EHandleSameItemMethod InHandleSameItemMethod)
{
    if (IsItemIndexValid(InType, InSlotIndex, InItemIndex))
    {
        if (IsItemValid(InItem))
        {
            // make sure item type is same and slot type
            if (GetItemType(InItem) == InType)
            {
                FStoredItem OldItem = GetItemInSlot(InType, InSlotIndex, InItemIndex);

                if (OldItem.Id == InItem.Id)
                {
                    if (InHandleSameItemMethod == EHandleSameItemMethod::Unequip)
                    {
                        FStoredItem EmptyItem = FStoredItem();

                        SetItemInSlot(InType, InSlotIndex, InItemIndex, EmptyItem);

                        EquippedItems.Remove(OldItem.Id);

                        BroadcastOnItemInSlotChanged(OldItem, EmptyItem, InType, InSlotIndex, InItemIndex);
                    }
                    else
                    {
                        SetItemInSlot(InType, InSlotIndex, InItemIndex, InItem);

                        EquippedItems.Remove(OldItem.Id);
                        EquippedItems.Add(InItem.Id);

                        BroadcastOnItemInSlotChanged(OldItem, InItem, InType, InSlotIndex, InItemIndex);
                    }
                }
                else
                {
                    int EqSlotIndex = GetEqSlotsIndex(InType);

                    for (int i = 0; i < EquipmentSlots[EqSlotIndex].Slots.Num(); ++i)
                    {
                        FEquipmentSlot Slot = EquipmentSlots[EqSlotIndex].Slots[i];

                        for (int j = 0; j < Slot.Items.Num(); ++j)
                        {
                            FStoredItem SItem = Slot.Items[j];
                            FStoredItem EmptyItem = FStoredItem();

                            if (InItem.Id == SItem.Id)
                            {
                                SetItemInSlot(InType, i, j, EmptyItem);

                                EquippedItems.Remove(OldItem.Id);

                                BroadcastOnItemInSlotChanged(OldItem, EmptyItem, InType, InSlotIndex, InItemIndex);
                            }
                        }
                    }

                    SetItemInSlot(InType, InSlotIndex, InItemIndex, InItem);

                    EquippedItems.Remove(OldItem.Id);
                    EquippedItems.Add(InItem.Id);

                    BroadcastOnItemInSlotChanged(OldItem, InItem, InType, InSlotIndex, InItemIndex);
                }

            }
        }
        else
        {
            FStoredItem OldItem = GetItemInSlot(InType, InSlotIndex, InItemIndex);
            if (IsItemValid(OldItem))
            {
                FStoredItem EmptyItem;

                SetItemInSlot(InType, InSlotIndex, InItemIndex, EmptyItem);

                EquippedItems.Remove(OldItem.Id);

                BroadcastOnItemInSlotChanged(OldItem, EmptyItem, InType, InSlotIndex, InItemIndex);
            }
        }
    }
}

int UEquipmentComponent::GetEqSlotsIndex(EItemType InType) const
{
    for (int i = 0; i < EquipmentSlots.Num(); ++i)
    {
        if (EquipmentSlots[i].Type == InType)
        {
            return i;
        }
    }

    return -1;
}

bool UEquipmentComponent::IsActiveItemIndex(EItemType InType, int InSlotIndex, int InItemIndex) const
{
    return GetActiveItemIndex(InType, InSlotIndex) == InItemIndex;
}


bool UEquipmentComponent::IsItemEquipped(FGuid InItemId) const
{
    return EquippedItems.Contains(InItemId);
}

bool UEquipmentComponent::IsItemActive(FGuid InItemId) const
{
    return ActiveItems.Find(InItemId) >= 0;
}

bool UEquipmentComponent::AreArrowsEquipped() const
{
    if (IsSlotHidden(EItemType::Arrows, 0))
    {
        return false;
    }
    else
    {
        FStoredItem Item = GetActiveItem(EItemType::Arrows, 0);
        if (IsItemValid(Item))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool UEquipmentComponent::CanBlock() const
{
    if (bIsInCombat)
    {
        return CombatType == ECombatType::Melee || CombatType == ECombatType::Unarmed;
    }
    else
    {
        return false;
    }
}

bool UEquipmentComponent::GetBlockValue(float& InValue) const
{
    FStoredItem Item = GetActiveItem(EItemType::Shield, 0);
    InValue = 0.0f;

    if (IsItemValid(Item))
    {
        if (!IsSlotHidden(EItemType::Shield, 0))
        {
            UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Item.ItemClass);
            IItemCanBlock* ItemCanBlock = Cast<IItemCanBlock>(ItemBase);
            if (ItemCanBlock != nullptr)
            {
                InValue = ItemCanBlock->GetBlockValue();
                return true;
            }
        }
    }

    Item = GetActiveItem(SelectedMainHandType, 0);

    if (IsItemValid(Item))
    {
        if (!IsSlotHidden(SelectedMainHandType, 0))
        {
            UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Item.ItemClass);
            IItemCanBlock* ItemCanBlock = Cast<IItemCanBlock>(ItemBase);
            if (ItemCanBlock != nullptr)
            {
                InValue = ItemCanBlock->GetBlockValue();
                return true;
            }
        }
    }
    return false;
}

bool UEquipmentComponent::IsWeaponEquipped() const
{
    return GetWeaponType() != EWeaponType::None;
}

void UEquipmentComponent::OnItemModified(FStoredItem InItem)
{
    EItemType ItemType;
    int SlotIndex;
    int ItemIndex;

    if (FindItem(InItem, ItemType, SlotIndex, ItemIndex))
    {
        UpdateItemInSlot(ItemType, SlotIndex, ItemIndex, InItem, EHandleSameItemMethod::Update);
    }

}

void UEquipmentComponent::OnGameLoaded()
{
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameUtils::IsValid(GameMode))
    {
        SelectedMainHandType = GameMode->GetSelectedMainHandSlotType();
        BuildEquipment(GameMode->GetEquipmentSlots());
        SetCombat(GameMode->IsInCombat());
    }
}

void UEquipmentComponent::UpdateDisplayedItem(EItemType InType, int InSlotIndex)
{
    FDisplayedItems* ItemsValue = DisplayedItems.Find(InType);

    if (ItemsValue != nullptr)
    {
        ADisplayedItem* DisplayedItem = ItemsValue->DisplayedItems[InSlotIndex];
        if (DisplayedItem != nullptr)
        {
            DisplayedItem->Destroy();
        }

        FStoredItem ItemData = GetActiveItem(InType, InSlotIndex);

        if (UKismetSystemLibrary::IsValidClass(ItemData.ItemClass))
        {
            UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), ItemData.ItemClass);

            IItemIsDisplayed* ItemIsDisplayed = Cast<IItemIsDisplayed>(ItemBase);

            if (ItemIsDisplayed != nullptr)
            {
                TSubclassOf<ADisplayedItem> DisplayedItemClass = ItemIsDisplayed->GetDisplayedItem();
                if (UKismetSystemLibrary::IsValidClass(DisplayedItemClass))
                {
                    FActorSpawnParameters SpawnParameters = FActorSpawnParameters();

                    SpawnParameters.Owner = GetOwner();
                    SpawnParameters.Instigator = GetOwner()->GetInstigator();

                    ADisplayedItem* SpawnedActor = 
                        GetWorld()->SpawnActor<ADisplayedItem>(
                            DisplayedItemClass, FTransform::Identity, SpawnParameters);

                    SpawnedActor->Init(this, InType, InSlotIndex);
                    ItemsValue->DisplayedItems[InSlotIndex] = SpawnedActor;
                }
            }
        }
    }

}

void UEquipmentComponent::SetSlotHidden(EItemType InType, int InSlotIndex, bool bInIsHidden)
{
    bool bIsSlotHidden = IsSlotHidden(InType, InSlotIndex);
    if (bIsSlotHidden != bInIsHidden)
    {
        int Index = GetEqSlotsIndex(InType);
        FEquipmentSlot& Slot = EquipmentSlots[Index].Slots[InSlotIndex];

        Slot.bIsHidden = bInIsHidden;

        int ActiveItemIndex = GetActiveItemIndex(InType, InSlotIndex);
        FStoredItem ActiveItem = GetItemInSlot(InType, InSlotIndex, ActiveItemIndex);

        OnSlotHiddenChanged.Broadcast(InType, InSlotIndex, ActiveItem, bInIsHidden);
    }
}



void UEquipmentComponent::BroadcastOnItemInSlotChanged(
    FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex)
{
    OnItemInSlotChanged.Broadcast(InOldItem, InNewItem, InType, InSlotIndex, InItemIndex);

    if (IsActiveItemIndex(InType, InSlotIndex, InItemIndex))
    {
        ActiveItemChanged(InOldItem, InNewItem, InType, InSlotIndex, InItemIndex);
    }
}



void UEquipmentComponent::ActiveItemChanged(
    FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex)
{
    if (InType == SelectedMainHandType)
    {
        UpdateCombatType();

        if (IsItemTwoHanded(InNewItem))
        {
            SetSlotHidden(EItemType::Shield, 0, true);
        }
        else
        {
            SetSlotHidden(EItemType::Shield, 0, false);
        }

        SetSlotHidden(
            EItemType::Arrows, 0, !(SelectedMainHandType == EItemType::RangeWeapon && GetCombatType() == ECombatType::Range));
    }

    if (InOldItem.Id != InNewItem.Id)
    {
        ActiveItems.Remove(InOldItem.Id);
        ActiveItems.Add(InNewItem.Id);

        UpdateDisplayedItem(InType, InSlotIndex);
        AttachDisplayedItem(InType, InSlotIndex);
    }

    OnActiveItemChanged.Broadcast(InOldItem, InNewItem, InType, InSlotIndex, InActiveIndex);
}

void UEquipmentComponent::UseActiveItemAtSlot(EItemType InType, int InSlotIndex)
{
    int ActiveItemIndex = GetActiveItemIndex(InType, InSlotIndex);
    if (IsItemIndexValid(InType, InSlotIndex, ActiveItemIndex))
    {
        int EqSlotsIndex = GetEqSlotsIndex(InType);
        FEquipmentSlot Slot = EquipmentSlots[EqSlotsIndex].Slots[InSlotIndex];

        if (!Slot.bIsHidden)
        {
            FStoredItem& SlotItem = Slot.Items[ActiveItemIndex];

            if (IsItemValid(SlotItem))
            {
                if (Inventory != nullptr)
                {
                    Inventory->UseItem(SlotItem.Id);
                }
                else
                {
                    UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), SlotItem.ItemClass);
                    ItemBase->UseItem(GetOwner());
                    
                    UItemBase* DefaultItem = Cast<UItemBase>(SlotItem.ItemClass->GetDefaultObject());

                    if (DefaultItem->GetItem().bIsConsumable)
                    {
                        SlotItem.Amount = SlotItem.Amount - 1;
                        UpdateItemInSlot(InType, InSlotIndex, ActiveItemIndex, SlotItem, EHandleSameItemMethod::Update);
                    }
                }
            }
        }
    }
}

bool UEquipmentComponent::FindItem(FStoredItem InItem, EItemType& OutType, int& OutSlotIndex, int& OutItemIndex) const
{
    EItemType ItemType = GetItemType(InItem);
    int EqSlotsIndex = GetEqSlotsIndex(ItemType);

    OutType = EItemType::None;
    OutSlotIndex = -1;
    OutItemIndex = -1;

    if (EqSlotsIndex >= 0)
    {
        const TArray<FEquipmentSlot>& Slots = EquipmentSlots[EqSlotsIndex].Slots;

        for(int i = 0; i < Slots.Num(); ++i)
        {
            for (int j = 0; j < Slots[i].Items.Num(); ++j)
            {
                if (Slots[i].Items[j].Id == InItem.Id)
                {
                    OutType = EquipmentSlots[EqSlotsIndex].Type;
                    OutSlotIndex = i;
                    OutItemIndex = j;
                    return true;
                }
            }
        }
    }

    return false;
}

void UEquipmentComponent::BuildEquipment(const TArray<FEquipmentSlots>& InEquipment)
{
    TArray<FEquipmentSlots> Equipment(InEquipment);

    // clear and rebuild displayed items array
    for (auto E : DisplayedItems)
    {
        const TArray<ADisplayedItem*>& DisplayedItemActors = E.Value.DisplayedItems;

        for (ADisplayedItem* DisplayedItem : DisplayedItemActors)
        {
            if (DisplayedItem != nullptr)
            {
                DisplayedItem->Destroy();
            }
        }

    }
    DisplayedItems.Empty();
    
    for (const FEquipmentSlots& Slots : Equipment)
    {
        FDisplayedItems Items;
        Items.DisplayedItems.Empty(Slots.Slots.Num());

        for (int i = 0; i < Slots.Slots.Num(); ++i)
        {
            Items.DisplayedItems.Add(nullptr);
        }

        DisplayedItems.Add(Slots.Type, Items);
    }

    // unequip all items
    for (int i = 0; i < EquipmentSlots.Num(); ++i)
    {
        const FEquipmentSlots& Slots = EquipmentSlots[i];
        int EqSlotsIndex = i;
        EItemType Type = Slots.Type;

        for (int j = 0; j < Slots.Slots.Num(); ++j)
        {
            int SlotIndex = j;
            const FEquipmentSlot& Slot = Slots.Slots[j];

            for (int k = 0; k < Slot.Items.Num(); ++k)
            {
                int ItemIndex = k;
                FStoredItem EmptyItem;

                if (IsItemValid(Slot.Items[k]))
                {
                    if (IsItemEquipped(Slot.Items[k].Id))
                    {
                        UpdateItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem, EHandleSameItemMethod::Unequip);
                    }
                    else
                    {
                        SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);
                    }
                }
                else
                {
                    SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);
                }
            }            
        }
    }

    // hide not selected main hand slot types
    for (EItemType Type : MainHandTypes)
    {
        const FEquipmentSlots& Slots = EquipmentSlots[GetEqSlotsIndex(Type)];
        EItemType SlotItemType = Slots.Type;

        for (int i = 0; i < Slots.Slots.Num(); ++i)
        {
            SetSlotHidden(SlotItemType, i, true);
            if (SlotItemType == SelectedMainHandType)
            {
                SetSlotHidden(SlotItemType, i, false);
            }
        }
    }
    
    for (int i = 0; i < Equipment.Num(); ++i)
    {
        int EqSlotsIndex = i;
        EItemType Type = Equipment[i].Type;

        for (int j = 0; j < Equipment[i].Slots.Num(); ++j)
        {
            int SlotIndex = j;
            SetSlotActiveIndex(Type, SlotIndex, Equipment[i].Slots[j].ActiveItemIndex);

            for (int k = 0; k < Equipment[i].Slots[j].Items.Num(); ++k)
            {
                int ItemIndex = k;
                FStoredItem Item = Equipment[i].Slots[j].Items[k];

                if (Inventory != nullptr)
                {
                    // If owner has inventory component, check only class, amount doesn't matter
                    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
                    {
                        int FoundItemIndex;
                        if (Item.Id.IsValid())
                        {
                            FoundItemIndex = Inventory->FindIndexById(Item.Id);
                        }
                        else
                        {
                            FoundItemIndex = Inventory->FindIndexByClass(Item.ItemClass);
                        }

                        if (FoundItemIndex >= 0)
                        {
                            FStoredItem InventoryItem = Inventory->GetItemAtIndex(FoundItemIndex);
                            UpdateItemInSlot(Type, SlotIndex, ItemIndex, InventoryItem, EHandleSameItemMethod::Update);
                        }
                    }
                }
                else
                {
                    if (IsItemValid(Item))
                    {
                        UpdateItemInSlot(Type, SlotIndex, ItemIndex, Item, EHandleSameItemMethod::Update);
                    }
                }
            }
        }
    }

    UpdateCombatType();

}


void UEquipmentComponent::SetCombat(bool bInValue)
{
    if (bIsInCombat != bInValue)
    {
        bIsInCombat = bInValue;
        AttachDisplayedItem(SelectedMainHandType, 0);
        if (SelectedMainHandType == EItemType::MeleeWeapon)
        {
            AttachDisplayedItem(EItemType::Shield, 0);
        }

        OnInCombatChanged.Broadcast(bIsInCombat);
    }
}

void UEquipmentComponent::UpdateCombatType()
{
    ECombatType PreviousCombatType = CombatType;
    EWeaponType PreviousWeaponType = WeaponType;

    FStoredItem WeaponItem = GetWeapon();
    if (IsItemValid(WeaponItem))
    {
        if (SelectedMainHandType == EItemType::Spell)
        {
            CombatType = ECombatType::Magic;
        } 
        else if (SelectedMainHandType == EItemType::MeleeWeapon)
        {
            CombatType = ECombatType::Melee;
        }
        else if (SelectedMainHandType == EItemType::RangeWeapon)
        {
            CombatType = ECombatType::Range;
        }

        WeaponType = Cast<UWeaponItem>(WeaponItem.ItemClass->GetDefaultObject())->GetWeaponType();

    }
    else
    {
        CombatType = ECombatType::Unarmed;
        WeaponType = EWeaponType::None;
    }

    if (CombatType != PreviousCombatType)
    {
        OnCombatTypeChanged.Broadcast(CombatType);
    }

    if (WeaponType != PreviousWeaponType)
    {
        OnWeaponTypeChanged.Broadcast(WeaponType);
    }
}

void UEquipmentComponent::SetItemInSlot(EItemType InType, int InSlotIndex, int InItemIndex, FStoredItem InItem)
{
    int EqSlotsIndex = GetEqSlotsIndex(InType);
    EquipmentSlots[EqSlotsIndex].Slots[InSlotIndex].Items[InItemIndex] = InItem;
}

void UEquipmentComponent::AttachDisplayedItem(EItemType InType, int InSlotIndex)
{
    FString EnumStr = GameUtils::GetEnumValueAsString("EItemType", InType);

    UE_LOG(LogTemp, Error, TEXT("AttachDisplayedItem  InType : %s"), *EnumStr);

    ADisplayedItem* DisplayedItem = GetDisplayedItem(InType, InSlotIndex);

    if (GameUtils::IsValid(DisplayedItem))
    {
        DisplayedItem->Attach();
    }
}

FStoredItem UEquipmentComponent::GetWeapon() const
{
    return GetItemInSlot(SelectedMainHandType, 0, GetActiveItemIndex(SelectedMainHandType, 0));
}

EItemType UEquipmentComponent::GetItemType(FStoredItem InItem) const
{
    UItemBase* ItemBase = Cast<UItemBase>(InItem.ItemClass->GetDefaultObject());
    return ItemBase->GetItem().Type;
}

bool UEquipmentComponent::IsItemValid(FStoredItem InItem) const
{
    return UKismetSystemLibrary::IsValidClass(InItem.ItemClass) && InItem.Amount > 0;
}

bool UEquipmentComponent::IsItemIndexValid(EItemType InType, int InSlotIndex, int InItemIndex) const
{
    int EqSlotsIndex = GetEqSlotsIndex(InType);
    if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
    {
        if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(InSlotIndex))
        {
            if (EquipmentSlots[EqSlotsIndex].Slots[InSlotIndex].Items.IsValidIndex(InItemIndex))
            {
                return true;
            }
        }
    }
    return false;
}



int UEquipmentComponent::GetActiveItemIndex(EItemType InType, int InSlotIndex) const
{
    return EquipmentSlots[GetEqSlotsIndex(InType)].Slots[InSlotIndex].ActiveItemIndex;
}

bool UEquipmentComponent::IsSlotIndexValid(EItemType InType, int InSlotIndex) const
{
    int EqSlotsIndex = GetEqSlotsIndex(InType);

    if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
    {
        if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(InSlotIndex))
        {
            return true;
        }
    }
    return false;
}

template <typename ElementType>
int UEquipmentComponent::GetNextArrayIndex(TArray<ElementType> InArray, int InIndex, bool bInForward) const
{
    if (bInForward)
    {
        if (InArray.IsValidIndex(InIndex + 1))
        {
            return InIndex + 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (InArray.IsValidIndex(InIndex - 1))
        {
            return InIndex - 1;
        }
        else
        {
            return InArray.Num() - 1;
        }
    }
}

bool UEquipmentComponent::IsItemTwoHanded(FStoredItem InItem) const
{
    if (UKismetSystemLibrary::IsValidClass(InItem.ItemClass))
    {
        IItemCanBeTwoHanded* ItemCanBeTwoHanded = 
            Cast<IItemCanBeTwoHanded>(NewObject<UItemBase>(GetOwner(), InItem.ItemClass));

        if (ItemCanBeTwoHanded != nullptr)
        {
            if (ItemCanBeTwoHanded->IsTwoHanded())
            {
                return true;
            }
        }
    }
    return  false;
}

