// Fill out your copyright notice in the Description page of Project Settings.


#include "HandDisplayedItem.h"
#include "Components/EquipmentComponent.h"

void AHandDisplayedItem::BeginPlay()
{
    Super::BeginPlay();
}

void AHandDisplayedItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    EquipmentComponent->OnSlotHiddenChanged.RemoveDynamic(this, &AHandDisplayedItem::OnSlotHiddenChanged);

    Super::EndPlay(EndPlayReason);
}

void AHandDisplayedItem::Init_Impl(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    Super::Init_Impl(InEquipmentComponent, InType, InSlotIndex);
    EquipmentComponent->OnSlotHiddenChanged.AddDynamic(this, &AHandDisplayedItem::OnSlotHiddenChanged);
}

void AHandDisplayedItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

FName AHandDisplayedItem::GetAttachmentSocket() const
{
    if (EquipmentComponent->IsSlotHidden(Type, SlotIndex))
    {
        return AttachmentSocket;
    }
    else
    {
        if (EquipmentComponent->IsInCombat())
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
    if (EquipmentComponent->IsInCombat())
    {
        if (InSlotType == Type && InSlotIndex == this->SlotIndex)
        {
            Attach();
        }
    }
}