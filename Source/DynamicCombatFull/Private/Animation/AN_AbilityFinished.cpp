// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_AbilityFinished.h"
#include "Components/AbilityComponent.h"
#include "GameCore/GameUtils.h"



void UAN_AbilityFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UAbilityComponent* AbilityComponent =
            Cast<UAbilityComponent>(Owner->GetComponentByClass(UAbilityComponent::StaticClass()));

        if (GameUtils::IsValid(AbilityComponent))
        {
            AbilityComponent->EndAbility(EAbilityEndResult::Finished);
        }
    }
}

