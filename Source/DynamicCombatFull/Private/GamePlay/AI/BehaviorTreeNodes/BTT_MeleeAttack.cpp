// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MeleeAttack.h"
#include "Interfaces/CanMeleeAttack.h"

UBTT_MeleeAttack::UBTT_MeleeAttack(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    AttackType(EMeleeAttackType::Light)
{
}

void UBTT_MeleeAttack::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    ICanMeleeAttack* CanMeleeAttack = Cast<ICanMeleeAttack>(ControlledPawn);

    if (CanMeleeAttack != nullptr)
    {
        float Duration = CanMeleeAttack->MeleeAttack(AttackType);

        FTimerHandle UnusedHandle;
        GetWorld()->GetTimerManager().SetTimer(
            UnusedHandle, this, &UBTT_MeleeAttack::Delayed_Execute, Duration, false);
    }
    else
    {
        FinishExecute(false);
    }
}

void UBTT_MeleeAttack::Delayed_Execute()
{
    FinishExecute(true);
}
