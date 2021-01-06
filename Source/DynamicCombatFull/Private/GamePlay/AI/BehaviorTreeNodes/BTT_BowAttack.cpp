// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BowAttack.h"
#include "Interfaces/IsArcher.h"

UBTT_BowAttack::UBTT_BowAttack(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer)
{
}


void UBTT_BowAttack::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    IIsArcher* IsArcher = Cast<IIsArcher>(ControlledPawn);
    if (IsArcher != nullptr)
    {
        float Duration = IsArcher->BowAttack();
        Duration = Duration + FMath::RandRange(1.0f, 2.0f);

        FTimerHandle UnusedHandle;
        GetWorld()->GetTimerManager().SetTimer(
            UnusedHandle, this, &UBTT_BowAttack::Delayed_Execute, Duration, false);
    }
    else
    {
        FinishExecute(false);
    }
}

void UBTT_BowAttack::Delayed_Execute()
{
    FinishExecute(true);
}
