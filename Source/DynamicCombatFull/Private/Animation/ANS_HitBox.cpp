// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_HitBox.h"
#include "GameCore/GameUtils.h"
#include "Components/CollisionHandlerComponent.h"

UANS_HitBox::UANS_HitBox()
    :
    CollisionPart(ECollisionPart::MainHandItem)
{
}

void UANS_HitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    ExecuteNotify(MeshComp, Animation, true);
}

void UANS_HitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ExecuteNotify(MeshComp, Animation, false);
}

void UANS_HitBox::ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UCollisionHandlerComponent* CollisionHandlerComponent =
            Cast<UCollisionHandlerComponent>(Owner->GetComponentByClass(UCollisionHandlerComponent::StaticClass()));

        if (GameUtils::IsValid(CollisionHandlerComponent))
        {
            if (bBegin)
            {
                CollisionHandlerComponent->ActivateCollision(CollisionPart);
            }
            else
            {
                CollisionHandlerComponent->DeactivateCollision();
            }

        }
    }
}