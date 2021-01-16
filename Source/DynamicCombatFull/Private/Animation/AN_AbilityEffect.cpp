// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_AbilityEffect.h"
#include "GameCore/GameUtils.h"
#include "Components/AbilityComponent.h"

void UAN_AbilityEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UAbilityComponent* AbilityComponent = 
            Cast<UAbilityComponent>(Owner->GetComponentByClass(UAbilityComponent::StaticClass()));

        if (GameUtils::IsValid(AbilityComponent))
        {
            AbilityComponent->AbilityEffect();
        }
    }
}