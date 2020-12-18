// Fill out your copyright notice in the Description page of Project Settings.


#include "HandDisplayedItem.h"
#include "Components/EquipmentComponent.h"

void AHandDisplayedItem::BeginPlay()
{
    Super::BeginPlay();

    GetEquipmentComponent()->OnSlotHiddenChanged.AddDynamic(this, &AHandDisplayedItem::OnSlotHiddenChanged);
}

void AHandDisplayedItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

FName AHandDisplayedItem::GetAttachmentSocket() const
{
    if (GetEquipmentComponent()->IsSlotHidden(Type, SlotIndex))
    {
        return AttachmentSocket;
    }
    else
    {
        if (GetEquipmentComponent()->IsInCombat())
        {
            return HandAttachmentSocket;
        }
        else
        {
            return AttachmentSocket;
        }
    }
}

void AHandDisplayedItem::OnSlotHiddenChanged(EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden)
{
    if (GetEquipmentComponent()->IsInCombat())
    {
        if (InSlotType == Type && InSlotIndex == this->SlotIndex)
        {
            Attach();
        }
    }
}