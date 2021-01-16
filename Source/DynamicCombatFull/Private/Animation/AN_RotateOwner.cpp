// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_RotateOwner.h"
#include "GameCore/GameUtils.h"
#include "Components/RotatingComponent.h"

UAN_RotateOwner::UAN_RotateOwner()
    :
    MaxDegreesPerSecond(720.0f),
    MaxPossibleRotation(180.0f)
{
}

void UAN_RotateOwner::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        URotatingComponent* RotatingComponent = 
            Cast<URotatingComponent>(Owner->GetComponentByClass(URotatingComponent::StaticClass()));

        if (GameUtils::IsValid(RotatingComponent))
        {
            RotatingComponent->StartRotatingWithLimit(MaxPossibleRotation, MaxDegreesPerSecond);
        }
    }
}