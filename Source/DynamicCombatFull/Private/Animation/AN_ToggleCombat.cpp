// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ToggleCombat.h"
#include "GameCore/GameUtils.h"
#include "Components/EquipmentComponent.h"

void UAN_ToggleCombat::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UEquipmentComponent* EquipmentComponent =
            Cast<UEquipmentComponent>(Owner->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (GameUtils::IsValid(EquipmentComponent))
        {
            EquipmentComponent->ToggleCombat();
        }
    }
}