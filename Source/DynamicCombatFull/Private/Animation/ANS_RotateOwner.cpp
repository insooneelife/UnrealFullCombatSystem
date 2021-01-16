// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RotateOwner.h"
#include "Components/RotatingComponent.h"
#include "GameCore/GameUtils.h"

UANS_RotateOwner::UANS_RotateOwner()
    :
    MaxDegreesPerSecond(720.0f)
{
}

void UANS_RotateOwner::NotifyBegin(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    ExecuteNotify(MeshComp, Animation, true);
}

void UANS_RotateOwner::NotifyEnd(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ExecuteNotify(MeshComp, Animation, false);
}


void UANS_RotateOwner::ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        URotatingComponent* RotatingComponent =
            Cast<URotatingComponent>(Owner->GetComponentByClass(URotatingComponent::StaticClass()));

        if (GameUtils::IsValid(RotatingComponent))
        {
            if (bBegin)
            {
                RotatingComponent->StartRotatingWithTime(10.0f, MaxDegreesPerSecond);
            }
            else
            {
                RotatingComponent->StopRotating();
            }

        }
    }
}