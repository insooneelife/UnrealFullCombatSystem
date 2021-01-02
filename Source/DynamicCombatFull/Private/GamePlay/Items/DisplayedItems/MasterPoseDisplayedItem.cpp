// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterPoseDisplayedItem.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"

bool AMasterPoseDisplayedItem::Attach()
{
    bool bResult = Super::Attach();

    if (bResult)
    {
        USkeletalMeshComponent* Comp = Cast<USkeletalMeshComponent>(GetPrimaryComponent());

        if (GameUtils::IsValid(Comp))
        {
            ACharacter* Character = Cast<ACharacter>(GetOwner());

            Comp->SetMasterPoseComponent(Character->GetMesh());

            return true;
        }
    }
    return false;
}