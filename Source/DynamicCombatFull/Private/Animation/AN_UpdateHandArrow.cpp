// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_UpdateHandArrow.h"
#include "GameCore/GameUtils.h"
#include "Components/EquipmentComponent.h"
#include "GamePlay/Items/DisplayedItems/BowDisplayedItem.h"


void UAN_UpdateHandArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UEquipmentComponent* EquipmentComponent =
            Cast<UEquipmentComponent>(Owner->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (GameUtils::IsValid(EquipmentComponent))
        {
            ABowDisplayedItem* BowDisplayedItem =
                Cast<ABowDisplayedItem>(EquipmentComponent->GetDisplayedItem(EItemType::RangeWeapon, 0));

            if (GameUtils::IsValid(BowDisplayedItem))
            {
                BowDisplayedItem->UpdateArrowAttachment(bAttachToHand);
                BowDisplayedItem->UpdateArrowVisibility(bVisible);
            }
        }
    }

}