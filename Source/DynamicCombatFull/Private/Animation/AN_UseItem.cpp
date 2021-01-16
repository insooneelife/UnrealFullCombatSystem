// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_UseItem.h"
#include "GameCore/GameUtils.h"
#include "Components/InventoryComponent.h"
#include "Components/EquipmentComponent.h"

UAN_UseItem::UAN_UseItem()
    :
    ItemType(EItemType::Tool)
{
}

void UAN_UseItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UEquipmentComponent* EquipmentComponent =
            Cast<UEquipmentComponent>(Owner->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (GameUtils::IsValid(EquipmentComponent))
        {
            UInventoryComponent* InventoryComponent =
                Cast<UInventoryComponent>(Owner->GetComponentByClass(UInventoryComponent::StaticClass()));

            if (GameUtils::IsValid(InventoryComponent))
            {
                FStoredItem StoredItem = EquipmentComponent->GetActiveItem(ItemType, 0);
                InventoryComponent->UseItem(StoredItem.Id);
            }
        }
    }
}