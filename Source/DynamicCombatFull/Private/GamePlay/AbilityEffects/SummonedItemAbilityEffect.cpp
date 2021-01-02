// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonedItemAbilityEffect.h"
#include "Kismet/KismetGuidLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/EquipmentComponent.h"
#include "Components/StateManagerComponent.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GameCore/GameUtils.h"

// Sets default values
ASummonedItemAbilityEffect::ASummonedItemAbilityEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Amount = 1;
}

// Called every frame
void ASummonedItemAbilityEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    ElapsedTime += DeltaTime;
}

void ASummonedItemAbilityEffect::Init(
    TSubclassOf<UItemBase> InClass,
    float InDuration,
    int InAmount,
    bool bInAutoSwap)
{
    ItemClass = InClass;
    Duration = InDuration;
    Amount = InAmount;
    bAutoSwap = bInAutoSwap;

    StateManager =
        Cast<UStateManagerComponent>(GetOwner()->GetComponentByClass(UStateManagerComponent::StaticClass()));

    EquipmentComponent =
        Cast<UEquipmentComponent>(GetOwner()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (GameUtils::IsValid(EquipmentComponent))
    {
        SummonWeapon();
    }
    else
    {
        Destroy();
    }
}

void ASummonedItemAbilityEffect::UnsummonWeapon()
{
    FStoredItem Item = EquipmentComponent->GetItemInSlot(GetCreatedItemType(), SlotIndex, ItemIndex);

    if (Item.Id == SummonedItem.Id)
    {
        EquipOldItem();
    }
    Destroy();
}

void ASummonedItemAbilityEffect::SummonWeapon()
{
    FStoredItem Item(UKismetGuidLibrary::NewGuid(), ItemClass, Amount);
    SummonedItem = Item;

    ItemIndex = GetActiveIndex();
    StoreItemBeforeSummon();

    EItemType CreatedItemType = GetCreatedItemType();
    EquipmentComponent->UpdateItemInSlot(
        CreatedItemType, SlotIndex, ItemIndex, SummonedItem, EHandleSameItemMethod::Update);

    if (bAutoSwap)
    {
        EquipmentComponent->SetMainHandType(CreatedItemType);
    }

    FTimerHandle AttemptToUnsummonTimerHandle;
    GetWorldTimerManager().SetTimer(
        AttemptToUnsummonTimerHandle, this, &ASummonedItemAbilityEffect::AttemptToUnsummon, Duration, false);

    FTimerHandle UnsummonWeaponTimerHandle;
    GetWorldTimerManager().SetTimer(
        UnsummonWeaponTimerHandle, this, &ASummonedItemAbilityEffect::UnsummonWeapon, Duration + 10.0f, false);
}

void ASummonedItemAbilityEffect::AttemptToUnsummon()
{
    if (GameUtils::IsValid(StateManager))
    {
        if (StateManager->GetState() == EState::Idle || StateManager->GetState() == EState::Dead)
        {
            UnsummonWeapon();
        }
        else
        {
            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(
                TimerHandle, this, &ASummonedItemAbilityEffect::AttemptToUnsummon, 0.1f, false);
        }
    }
    else
    {
        UnsummonWeapon();
    }
}

void ASummonedItemAbilityEffect::StoreItemBeforeSummon()
{
    ItemBeforeSummon = EquipmentComponent->GetItemInSlot(GetCreatedItemType(), SlotIndex, ItemIndex);
}

void ASummonedItemAbilityEffect::EquipOldItem()
{
    EItemType CreatedItemType = GetCreatedItemType();
    if (IsItemInInventory(ItemBeforeSummon))
    {
        EquipmentComponent->UpdateItemInSlot(
            CreatedItemType, SlotIndex, ItemIndex, SummonedItem, EHandleSameItemMethod::Update);
    }
    else
    {
        EquipmentComponent->UpdateItemInSlot(
            CreatedItemType, SlotIndex, ItemIndex, FStoredItem(), EHandleSameItemMethod::Update);
    }
}

int ASummonedItemAbilityEffect::GetActiveIndex() const
{
    const TArray<FEquipmentSlots>& EquipmentSlots = EquipmentComponent->GetEquipmentSlots();

    EItemType CreatedItemType = GetCreatedItemType();
    int Index = EquipmentComponent->GetEqSlotsIndex(CreatedItemType);

    return EquipmentSlots[Index].Slots[SlotIndex].ActiveItemIndex;
}

EItemType ASummonedItemAbilityEffect::GetCreatedItemType() const
{
    UItemBase* DefaultItemBase = Cast<UItemBase>(SummonedItem.ItemClass->GetDefaultObject(true));

    return DefaultItemBase->GetItem().Type;
}

bool ASummonedItemAbilityEffect::IsItemInInventory(const FStoredItem& Item) const
{
    UInventoryComponent* Inventory = 
        Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));

    if (GameUtils::IsValid(Inventory))
    {
        int Index = Inventory->FindIndexById(Item.Id);

        if (Index >= 0)
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

