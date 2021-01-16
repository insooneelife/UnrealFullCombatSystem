// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SetDisplayedItemVisibility.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"

UANS_SetDisplayedItemVisibility::UANS_SetDisplayedItemVisibility()
    :
    ItemType(EItemType::RangeWeapon),
    bHasToBeInCombat(false),
    bHideThenShow(true)
{
}

void UANS_SetDisplayedItemVisibility::NotifyBegin(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    ExecuteNotify(MeshComp, Animation, false);
}

void UANS_SetDisplayedItemVisibility::NotifyEnd(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ExecuteNotify(MeshComp, Animation, false);
}

void UANS_SetDisplayedItemVisibility::ExecuteNotify(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UEquipmentComponent* EquipmentComponent =
            Cast<UEquipmentComponent>(Owner->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (GameUtils::IsValid(EquipmentComponent))
        {
            if (bHasToBeInCombat)
            {
                if (EquipmentComponent->IsInCombat())
                {
                    SetItemVisibility(EquipmentComponent, bBegin);
                }
            }
            else
            {
                SetItemVisibility(EquipmentComponent, bBegin);
            }
        }
    }
}

void UANS_SetDisplayedItemVisibility::SetItemVisibility(UEquipmentComponent* EquipmentComponent, bool bVisible)
{
    if (!EquipmentComponent->IsSlotHidden(ItemType, 0))
    {
        ADisplayedItem* DisplayedItem = EquipmentComponent->GetDisplayedItem(ItemType, 0);
        if (GameUtils::IsValid(DisplayedItem))
        {
            if (bVisible)
            {
                DisplayedItem->SetActorHiddenInGame(bHideThenShow);
            }
            else
            {
                DisplayedItem->SetActorHiddenInGame(!bHideThenShow);
            }
        }
    }
}