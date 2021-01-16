// Fill out your copyright notice in the Description page of Project Settings.


#include "HandDisplayedItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/EquipmentComponent.h"

AHandDisplayedItem::AHandDisplayedItem()
{
    RootComponent = StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->ComponentTags.Add("Dissolve");
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AHandDisplayedItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    EquipmentComponent->OnSlotHiddenChanged.RemoveDynamic(this, &AHandDisplayedItem::OnSlotHiddenChanged);

    Super::EndPlay(EndPlayReason);
}

void AHandDisplayedItem::NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    Super::NativeInit(InEquipmentComponent, InType, InSlotIndex);
    EquipmentComponent->OnSlotHiddenChanged.AddDynamic(this, &AHandDisplayedItem::OnSlotHiddenChanged);
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