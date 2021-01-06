// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_StopMovement.h"
#include "AIController.h"

void UBTT_StopMovement::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    OwnerController->StopMovement();
    FinishExecute(true);
}
