// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Roll.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/AI/AICharacter.h"

UBTT_Roll::UBTT_Roll(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    Direction(EDirection::Back)
{
}

void UBTT_Roll::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    AAICharacter* Character = Cast<AAICharacter>(ControlledPawn);

    if (GameUtils::IsValid(Character))
    {
        float Duration = Character->Roll(Direction);
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBTT_Roll::Delayed_Execute, Duration, false);
    }
    else
    {
        FinishExecute(false);
    }    
}

void UBTT_Roll::Delayed_Execute()
{
    FinishExecute(true);
}
