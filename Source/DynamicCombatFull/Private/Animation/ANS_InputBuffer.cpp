// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_InputBuffer.h"
#include "Components/InputBufferComponent.h"
#include "GameCore/GameUtils.h"

void UANS_InputBuffer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    ExecuteNotify(MeshComp, Animation, true);
}

void UANS_InputBuffer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ExecuteNotify(MeshComp, Animation, false);
}

void UANS_InputBuffer::ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UInputBufferComponent* InputBufferComponent =
            Cast<UInputBufferComponent>(Owner->GetComponentByClass(UInputBufferComponent::StaticClass()));

        if (InputBufferComponent != nullptr)
        {
            if (bBegin)
            {
                InputBufferComponent->OpenInputBuffer();
            }
            else
            {
                InputBufferComponent->CloseInputBuffer();
            }

        }
    }
}