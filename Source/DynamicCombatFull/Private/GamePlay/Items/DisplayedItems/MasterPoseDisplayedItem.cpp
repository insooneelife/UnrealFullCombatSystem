// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterPoseDisplayedItem.h"
#include "GameFramework/Character.h"

void AMasterPoseDisplayedItem::BeginPlay()
{
    Super::BeginPlay();
}


void AMasterPoseDisplayedItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AMasterPoseDisplayedItem::NotifyActorBeginOverlap(AActor* OtherActor)
{

}

void AMasterPoseDisplayedItem::Tick(float DeltaTime)
{
}

bool AMasterPoseDisplayedItem::Attach()
{
    bool bResult = Super::Attach();

    if (bResult)
    {
        USkeletalMeshComponent* Comp = Cast<USkeletalMeshComponent>(GetPrimaryComponent());
        if (Comp != nullptr)
        {
            ACharacter* Character = Cast<ACharacter>(GetOwner());

            Comp->SetMasterPoseComponent(Character->GetMesh());

            return true;
        }
    }
    return false;
}