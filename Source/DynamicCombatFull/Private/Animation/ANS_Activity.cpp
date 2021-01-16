// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Activity.h"
#include "GameCore/GameUtils.h"
#include "Components/StateManagerComponent.h"

UANS_Activity::UANS_Activity()
    :
    Activity(EActivity::IsImmortal)
{
}

void UANS_Activity::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    ExecuteNotify(MeshComp, Animation, true);
}

void UANS_Activity::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ExecuteNotify(MeshComp, Animation, false);
}


void UANS_Activity::ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UStateManagerComponent* StateManagerComponent =
            Cast<UStateManagerComponent>(Owner->GetComponentByClass(UStateManagerComponent::StaticClass()));

        if (GameUtils::IsValid(StateManagerComponent))
        {
            if (bBegin)
            {
                StateManagerComponent->SetActivity(Activity, true);
            }
            else
            {
                StateManagerComponent->SetActivity(Activity, false);
            }
        }
    }
}