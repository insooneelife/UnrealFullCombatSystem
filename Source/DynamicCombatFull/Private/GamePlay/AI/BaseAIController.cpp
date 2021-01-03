// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AICharacter.h"

void ABaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    PossesedAICharacter = Cast<AAICharacter>(InPawn);

    if (PossesedAICharacter->IsValidLowLevel())
    {
        //PossesedAICharacter->Beha
    }
}

void ABaseAIController::OnInCombatChanged(bool bInIsInCombat)
{
}

void ABaseAIController::UpdateTarget()
{
}

void ABaseAIController::SetTarget(AActor* NewTarget)
{
}

void ABaseAIController::SetIsInCombat(bool bValue)
{
}

void ABaseAIController::UpdateSenseTarget()
{
}

bool ABaseAIController::IsEnemy(const FAIStimulus& InAIStimulus, AActor* InActor) const
{
    return false;
}
