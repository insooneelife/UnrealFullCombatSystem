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


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipmentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto E : DisplayedItems)
    {
        const FDisplayedItems& Item = E.Value;

        for (ADisplayedItem* DisplayItem : Item.DisplayedItems)
        {
            if (DisplayItem->IsValidLowLevel())
            {
                DisplayItem->Destroy();
            }
        }
    }

    DisplayedItems.Empty();

    if (Inventory->IsValidLowLevel())
    {
        Inventory->OnItemRemoved.RemoveDynamic(this, &UEquipmentComponent::OnItemModified);
        Inventory->OnItemAdded.RemoveDynamic(this, &UEquipmentComponent::OnItemModified);
    }

    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode->IsValidLowLevel())
        {
            GameMode->OnGameLoaded.RemoveDynamic(this, &UEquipmentComponent::OnGameLoaded);
        }
    }

    Super::EndPlay(EndPlayReason);
}

void UEquipmentComponent::Init()
{
    // inventory events
    Inventory = Cast <UInventoryComponent> (GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));

    if (Inventory->IsValidLowLevel())
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
        if (GameMode != nullptr)
        {
            GameMode->OnGameLoaded.AddDynamic(this, &UEquipmentComponent::OnGameLoaded);
        }        
    }
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
    if (GameMode != nullptr)
    {
        SelectedMainHandType = GameMode->GetSelectedMainHandSlotType();
        BuildEquipment(GameMode->GetEquipmentSlots());
        SetCombat(GameMode->IsInCombat());
    }
}

void UEquipmentComponent::UpdateDisplayedItem(EItemType Type, int SlotIndex)
{
    FDisplayedItems* ItemsValue = DisplayedItems.Find(Type);

    if (ItemsValue != nullptr)
    {
        ADisplayedItem* DisplayedItem = ItemsValue->DisplayedItems[SlotIndex];
        if(DisplayedItem->IsValidLowLevel())
        {
            DisplayedItem->Destroy();
        }

        FStoredItem ItemData = GetActiveItem(Type, SlotIndex);

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

                    SpawnedActor->Init(this, Type, SlotIndex);
                    ItemsValue->DisplayedItems[SlotIndex] = SpawnedActor;
                }
            }
        }
    }

}

ADisplayedItem* UEquipmentComponent::GetDisplayedItem(EItemType Type, int SlotIndex) const
{
    if (DisplayedItems.Contains(Type))
    {
        return DisplayedItems[Type].DisplayedItems[SlotIndex];
    }
    return nullptr;
}

bool UEquipmentComponent::IsItemEquipped(FGuid ItemId) const
{
    return EquippedItems.Contains(ItemId);
}

FStoredItem UEquipmentComponent::GetActiveItem(EItemType Type, int SlotIndex) const
{
    int Index = GetEqSlotsIndex(Type);
    const FEquipmentSlot& Slot = EquipmentSlots[Index].Slots[SlotIndex];
    return Slot.Items[Slot.ActiveItemIndex];
}

bool UEquipmentComponent::IsSlotHidden(EItemType Type, int SlotIndex) const
{
    if (IsSlotIndexValid(Type, SlotIndex))
    {
        int Index = GetEqSlotsIndex(Type);
        const FEquipmentSlot& Slot = EquipmentSlots[Index].Slots[SlotIndex];
        return Slot.bIsHidden;
    }

    return false;
}

void UEquipmentComponent::SetSlotHidden(EItemType Type, int SlotIndex, bool bIsHidden)
{
    bool bIsSlotHidden = IsSlotHidden(Type, SlotIndex);
    if (bIsSlotHidden != bIsHidden)
    {
        int Index = GetEqSlotsIndex(Type);
        FEquipmentSlot& Slot = EquipmentSlots[Index].Slots[SlotIndex];

        Slot.bIsHidden = bIsHidden;

        int ActiveItemIndex = GetActiveItemIndex(Type, SlotIndex);
        FStoredItem ActiveItem = GetItemInSlot(Type, SlotIndex, ActiveItemIndex);

        OnSlotHiddenChanged.Broadcast(Type, SlotIndex, ActiveItem, bIsHidden);
    }
}

void UEquipmentComponent::UpdateItemInSlot(
    EItemType Type, int SlotIndex, int ItemIndex, FStoredItem Item, EHandleSameItemMethod HandleSameItemMethod)
{
    if (IsItemIndexValid(Type, SlotIndex, ItemIndex))
    {
        if (IsItemValid(Item))
        {
            // make sure item type is same and slot type
            if (GetItemType(Item) == Type)
            {
                FStoredItem OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);

                if (OldItem.Id == Item.Id)
                {
                    if (HandleSameItemMethod == EHandleSameItemMethod::Unequip)
                    {
                        FStoredItem EmptyItem = FStoredItem();

                        SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);

                        EquippedItems.Remove(OldItem.Id);

                        BroadcastOnItemInSlotChanged(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
                    }
                    else
                    {
                        SetItemInSlot(Type, SlotIndex, ItemIndex, Item);

                        EquippedItems.Remove(OldItem.Id);
                        EquippedItems.Add(Item.Id);

                        BroadcastOnItemInSlotChanged(OldItem, Item, Type, SlotIndex, ItemIndex);
                    }
                }
                else
                {
                    int EqSlotIndex = GetEqSlotsIndex(Type);

                    for (int i = 0; i < EquipmentSlots[EqSlotIndex].Slots.Num(); ++i)
                    {
                        FEquipmentSlot Slot = EquipmentSlots[EqSlotIndex].Slots[i];

                        for (int j = 0; j < Slot.Items.Num(); ++j)
                        {
                            FStoredItem SItem = Slot.Items[j];
                            FStoredItem EmptyItem = FStoredItem();

                            if (Item.Id == SItem.Id)
                            {
                                SetItemInSlot(Type, i, j, EmptyItem);

                                EquippedItems.Remove(OldItem.Id);

                                BroadcastOnItemInSlotChanged(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
                            }
                        }
                    }
                    //

                    SetItemInSlot(Type, SlotIndex, ItemIndex, Item);

                    EquippedItems.Remove(OldItem.Id);
                    EquippedItems.Add(Item.Id);

                    BroadcastOnItemInSlotChanged(OldItem, Item, Type, SlotIndex, ItemIndex);
                }

            }
        }
        else
        {
            FStoredItem OldItem = GetItemInSlot(Type, SlotIndex, ItemIndex);
            if (IsItemValid(OldItem))
            {
                FStoredItem EmptyItem;

                SetItemInSlot(Type, SlotIndex, ItemIndex, EmptyItem);

                EquippedItems.Remove(OldItem.Id);

                BroadcastOnItemInSlotChanged(OldItem, EmptyItem, Type, SlotIndex, ItemIndex);
            }
        }
    }
}

void UEquipmentComponent::BroadcastOnItemInSlotChanged(
    FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ItemIndex)
{
    OnItemInSlotChanged.Broadcast(OldItem, NewItem, Type, SlotIndex, ItemIndex);

    if (IsActiveItemIndex(Type, SlotIndex, ItemIndex))
    {
        ActiveItemChanged(OldItem, NewItem, Type, SlotIndex, ItemIndex);
    }
}

int UEquipmentComponent::GetEqSlotsIndex(EItemType Type) const
{
    for (int i = 0; i < EquipmentSlots.Num(); ++i)
    {
        if (EquipmentSlots[i].Type == Type)
        {
            return i;
        }
    }

    return -1;
}

void UEquipmentComponent::ActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex)
{
    if (Type == SelectedMainHandType)
    {
        UpdateCombatType();

        if (IsItemTwoHanded(NewItem))
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

    if (OldItem.Id != NewItem.Id)
    {
        ActiveItems.Remove(OldItem.Id);
        ActiveItems.Add(NewItem.Id);

        UpdateDisplayedItem(Type, SlotIndex);
        AttachDisplayedItem(Type, SlotIndex);
    }

    OnActiveItemChanged.Broadcast(OldItem, NewItem, Type, SlotIndex, ActiveIndex);
}

void UEquipmentComponent::SwitchSlotActiveIndex(EItemType Type, int SlotIndex, bool Forward, bool bIgnoreEmptyItems)
{
    if (IsSlotIndexValid(Type, SlotIndex))
    {
        int ActiveItemIndex = GetActiveItemIndex(Type, SlotIndex);

        int EqSlotsIndex = GetEqSlotsIndex(Type);

        FEquipmentSlot& EqSlot = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex];
        const TArray<FStoredItem>& Items = EqSlot.Items;

        int NewIndex = GetNextArrayIndex(Items, ActiveItemIndex, Forward);

        if (bIgnoreEmptyItems)
        {
            for (int i = 1; i < Items.Num(); ++i)
            {
                if (IsItemValid(Items[NewIndex]))
                {
                    EqSlot.ActiveItemIndex = NewIndex;
                    ActiveItemChanged(EqSlot.Items[ActiveItemIndex], EqSlot.Items[NewIndex], Type, SlotIndex, NewIndex);
                }
                else
                {
                    NewIndex = GetNextArrayIndex(Items, NewIndex, Forward);
                }
            }
        }
        else
        {
            EqSlot.ActiveItemIndex = NewIndex;
            ActiveItemChanged(EqSlot.Items[ActiveItemIndex], EqSlot.Items[NewIndex], Type, SlotIndex, NewIndex);
        }
    }
}

void UEquipmentComponent::SwitchMainHandType(bool bForward)
{
    int SelectedIndex = MainHandTypes.Find(SelectedMainHandType);

    if (SelectedIndex >= 0)
    {
        int NextIndex = GetNextArrayIndex(MainHandTypes, SelectedIndex, bForward);

        SetMainHandType(MainHandTypes[NextIndex]);
    }
}

FStoredItem UEquipmentComponent::GetWeapon() const
{
    return GetItemInSlot(SelectedMainHandType, 0, GetActiveItemIndex(SelectedMainHandType, 0));
}

EItemType UEquipmentComponent::GetSelectedMainHandType() const 
{
    return SelectedMainHandType; 
}

bool UEquipmentComponent::IsItemActive(FGuid ItemId) const
{
    return ActiveItems.Find(ItemId) >= 0;
}

bool UEquipmentComponent::IsActiveItemIndex(EItemType Type, int SlotIndex, int ItemIndex)
{
    return GetActiveItemIndex(Type, SlotIndex) == ItemIndex;
}

void UEquipmentComponent::UseActiveItemAtSlot(EItemType Type, int SlotIndex)
{
    int ActiveItemIndex = GetActiveItemIndex(Type, SlotIndex);
    if (IsItemIndexValid(Type, SlotIndex, ActiveItemIndex))
    {
        int EqSlotsIndex = GetEqSlotsIndex(Type);
        FEquipmentSlot Slot = EquipmentSlots[EqSlotsIndex].Slots[SlotIndex];

        if (!Slot.bIsHidden)
        {
            FStoredItem& SlotItem = Slot.Items[ActiveItemIndex];

            if (IsItemValid(SlotItem))
            {
                if (Inventory->IsValidLowLevel())
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
                        UpdateItemInSlot(Type, SlotIndex, ActiveItemIndex, SlotItem, EHandleSameItemMethod::Update);
                    }

                }
            }
        }
    }

    
}

bool UEquipmentComponent::FindItem(FStoredItem Item, EItemType& OutType, int& OutSlotIndex, int& OutItemIndex)
{
    EItemType ItemType = GetItemType(Item);
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
                if (Slots[i].Items[j].Id == Item.Id)
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
            if(DisplayedItem->IsValidLowLevel())
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

                if (Inventory->IsValidLowLevel())
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

FStoredItem UEquipmentComponent::GetItemInSlot(EItemType Type, int SlotIndex, int ItemIndex) const
{
    if (IsItemIndexValid(Type, SlotIndex, ItemIndex))
    {
        return EquipmentSlots[GetEqSlotsIndex(Type)].Slots[SlotIndex].Items[ItemIndex];
    }
    else
    {
        return FStoredItem();
    }
}

void UEquipmentComponent::SetSlotActiveIndex(EItemType Type, int SlotIndex, int NewActiveIndex)
{
    if (IsItemIndexValid(Type, SlotIndex, NewActiveIndex))
    {
        int ActiveItemIndex = GetActiveItemIndex(Type, SlotIndex);

        int EqSlotIndex = GetEqSlotsIndex(Type);

        if (ActiveItemIndex != NewActiveIndex &&
            ActiveItemIndex >= 0 &&
            NewActiveIndex >= 0)
        {
            FEquipmentSlot& EqSlot = EquipmentSlots[EqSlotIndex].Slots[SlotIndex];
            EqSlot.ActiveItemIndex = NewActiveIndex;
            ActiveItemChanged(EqSlot.Items[ActiveItemIndex], EqSlot.Items[NewActiveIndex], Type, SlotIndex, NewActiveIndex);
        }
    }
}

void UEquipmentComponent::SetMainHandType(EItemType Type)
{
    if (MainHandTypes.Find(Type) >= 0)
    {
        if (SelectedMainHandType != Type)
        {
            EItemType PreviousType = SelectedMainHandType;
            SelectedMainHandType = Type;

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

void UEquipmentComponent::SetCombat(bool bValue)
{
    if (bIsInCombat != bValue)
    {
        bIsInCombat = bValue;
        AttachDisplayedItem(SelectedMainHandType, 0);

        AttachDisplayedItem(EItemType::Shield, 0);

        OnInCombatChanged.Broadcast(bIsInCombat);
    }
}

ECombatType UEquipmentComponent::GetCombatType() const
{
    return CombatType;
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
}

bool UEquipmentComponent::GetBlockValue(float& Value) const
{
    FStoredItem Item = GetActiveItem(EItemType::Shield, 0);
    Value = 0.0f;

    if (IsItemValid(Item))
    {
        if (!IsSlotHidden(EItemType::Shield, 0))
        {
            UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Item.ItemClass);
            IItemCanBlock* ItemCanBlock = Cast<IItemCanBlock>(ItemBase);
            if (ItemCanBlock != nullptr)
            {
                Value = ItemCanBlock->GetBlockValue();
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
                Value = ItemCanBlock->GetBlockValue();
                return true;
            }
        }
    }
    return false;
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


EWeaponType UEquipmentComponent::GetWeaponType() const 
{ 
    return WeaponType; 
}

bool UEquipmentComponent::IsWeaponEquipped() const
{
    return GetWeaponType() != EWeaponType::None;
}

const TArray<FEquipmentSlots>& UEquipmentComponent::GetEquipmentSlots() const { return EquipmentSlots; }

EItemType UEquipmentComponent::GetItemType(FStoredItem Item) const
{
    UItemBase* ItemBase = Cast<UItemBase>(Item.ItemClass->GetDefaultObject());
    return ItemBase->GetItem().Type;
}

bool UEquipmentComponent::IsItemValid(FStoredItem Item) const
{
    return UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount > 0;
}

bool UEquipmentComponent::IsItemIndexValid(EItemType Type, int SlotIndex, int ItemIndex) const
{
    int EqSlotsIndex = GetEqSlotsIndex(Type);
    if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
    {
        if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
        {
            if (EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items.IsValidIndex(ItemIndex))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void UEquipmentComponent::SetItemInSlot(EItemType Type, int SlotIndex, int ItemIndex, FStoredItem Item)
{
    int EqSlotsIndex = GetEqSlotsIndex(Type);
    EquipmentSlots[EqSlotsIndex].Slots[SlotIndex].Items[ItemIndex] = Item;
}

int UEquipmentComponent::GetActiveItemIndex(EItemType Type, int SlotIndex) const
{
    return EquipmentSlots[GetEqSlotsIndex(Type)].Slots[SlotIndex].ActiveItemIndex;
}

bool UEquipmentComponent::IsSlotIndexValid(EItemType Type, int SlotIndex) const
{
    int EqSlotsIndex = GetEqSlotsIndex(Type);

    if (EquipmentSlots.IsValidIndex(EqSlotsIndex))
    {
        if (EquipmentSlots[EqSlotsIndex].Slots.IsValidIndex(SlotIndex))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

template <typename ElementType>
int UEquipmentComponent::GetNextArrayIndex(TArray<ElementType> Array, int Index, bool bForward) const
{
    if (bForward)
    {
        if (Array.IsValidIndex(Index + 1))
        {
            return Index + 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (Array.IsValidIndex(Index - 1))
        {
            return Index - 1;
        }
        else
        {
            return Array.Num() - 1;
        }
    }
}

void UEquipmentComponent::AttachDisplayedItem(EItemType Type, int SlotIndex)
{
    ADisplayedItem* DisplayedItem = GetDisplayedItem(Type, SlotIndex);

    if (DisplayedItem->IsValidLowLevel())
    {
        DisplayedItem->Attach();
    }
}

bool UEquipmentComponent::IsItemTwoHanded(FStoredItem Item) const
{
    if (UKismetSystemLibrary::IsValidClass(Item.ItemClass))
    {
        IItemCanBeTwoHanded* ItemCanBeTwoHanded = Cast<IItemCanBeTwoHanded>(NewObject<UItemBase>(GetOwner(), Item.ItemClass));

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

