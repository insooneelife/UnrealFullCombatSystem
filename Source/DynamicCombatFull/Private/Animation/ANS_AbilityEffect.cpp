// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AbilityEffect.h"
#include "GameCore/GameUtils.h"
#include "Components/AbilityComponent.h"
#include "Components/InputBufferComponent.h"

UANS_AbilityEffect::UANS_AbilityEffect()
    :
    bUseInputBuffer(true)
{
}

void UANS_AbilityEffect::NotifyBegin(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    ExecuteNotify(MeshComp, Animation, true);
}

void UANS_AbilityEffect::NotifyEnd(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ExecuteNotify(MeshComp, Animation, false);
}


void UANS_AbilityEffect::ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin)
{
    AActor* Owner = MeshComp->GetOwner();

    if (GameUtils::IsValid(Owner))
    {
        UAbilityComponent* AbilityComponent =
            Cast<UAbilityComponent>(Owner->GetComponentByClass(UAbilityComponent::StaticClass()));

        if (GameUtils::IsValid(AbilityComponent))
        {
            if (bBegin)
            {
                AbilityComponent->AbilityEffect();
            }
            else
            {
                AbilityComponent->EndAbility(EAbilityEndResult::Finished);
            }

            if (bUseInputBuffer)
            {
                if (AbilityComponent->CanAbilityBeCancelled())
                {
                    UInputBufferComponent* InputBufferComponent =
                        Cast<UInputBufferComponent>(Owner->GetComponentByClass(UInputBufferComponent::StaticClass()));

                    if (GameUtils::IsValid(InputBufferComponent))
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
        }

    }
}