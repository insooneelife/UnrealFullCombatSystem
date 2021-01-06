// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PlayMontageByAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/MontageManagerComponent.h"
#include "GameCore/GameUtils.h"

UBTT_PlayMontageByAction::UBTT_PlayMontageByAction(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    PlayRate(1.0f)
{
}

void UBTT_PlayMontageByAction::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    UMontageManagerComponent* MontageManager =
        Cast<UMontageManagerComponent>(ControlledPawn->GetComponentByClass(UMontageManagerComponent::StaticClass()));

    if (GameUtils::IsValid(MontageManager))
    {
        int LastIndex = MontageManager->GetMontageActionLastIndex(Action);
        int RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, LastIndex);
        int FinalIndex = bUseRandomIndex ? RandomIndex : Index;

        UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(Action, FinalIndex);

        if (GameUtils::IsValid(AnimMontage))
        {
            ACharacter* Character = Cast<ACharacter>(ControlledPawn);

            if (GameUtils::IsValid(Character))
            {
                float Duration = Character->PlayAnimMontage(AnimMontage);

                GameUtils::SetTimerRetriggerable(
                    GetWorld()->GetTimerManager(),
                    RetriggerableDelayTimerHandle,
                    FTimerDelegate::CreateUObject(this, &UBTT_PlayMontageByAction::Delayed_Execute),
                    Duration,
                    false);
            }
        }
        else
        {
            FinishExecute(false);
        }
    }
    else
    {
        FinishExecute(false);
    }

}

void UBTT_PlayMontageByAction::Delayed_Execute()
{
    FinishExecute(true);
}
