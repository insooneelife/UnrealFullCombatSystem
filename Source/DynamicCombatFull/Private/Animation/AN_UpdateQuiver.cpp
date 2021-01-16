// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_UpdateQuiver.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Items/DisplayedItems/QuiverDisplayedItem.h"


void UAN_UpdateQuiver::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UEquipmentComponent* EquipmentComponent =
            Cast<UEquipmentComponent>(Owner->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (GameUtils::IsValid(EquipmentComponent))
        {
            AQuiverDisplayedItem* DisplayedItem =
                Cast<AQuiverDisplayedItem>(EquipmentComponent->GetDisplayedItem(EItemType::Arrows, 0));
            
            if (GameUtils::IsValid(DisplayedItem))
            {
                DisplayedItem->UpdateQuiver();
            }
        }
    }
}
