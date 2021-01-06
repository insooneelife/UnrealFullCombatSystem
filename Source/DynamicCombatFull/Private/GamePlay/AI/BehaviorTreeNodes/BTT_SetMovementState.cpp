// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetMovementState.h"
#include "GameCore/GameUtils.h"
#include "Components/MovementSpeedComponent.h"

UBTT_SetMovementState::UBTT_SetMovementState(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    MovementState(EMovementState::Jog)
{
}

void UBTT_SetMovementState::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    UMovementSpeedComponent* MovementSpeed = 
        Cast<UMovementSpeedComponent>(ControlledPawn->GetComponentByClass(UMovementSpeedComponent::StaticClass()));

    if (GameUtils::IsValid(MovementSpeed))
    {
        MovementSpeed->SetMovementState(MovementState);
        FinishExecute(true);
    }
    else
    {
        FinishExecute(true);
    }
}
