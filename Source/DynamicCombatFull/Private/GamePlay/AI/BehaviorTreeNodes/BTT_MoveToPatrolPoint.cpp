// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_MoveToPatrolPoint.h"
#include "AIController.h"
#include "GameCore/GameUtils.h"
#include "Components/PatrolComponent.h"
#include "GamePlay/PatrolPathActor.h"


UBTT_MoveToPatrolPoint::UBTT_MoveToPatrolPoint(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    AcceptanceRadius(100.0f)
{
}

void UBTT_MoveToPatrolPoint::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceCreated(OwnerComp);
}

void UBTT_MoveToPatrolPoint::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceDestroyed(OwnerComp);

    AIOwner->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_MoveToPatrolPoint::OnMoveCompleted);
}

void UBTT_MoveToPatrolPoint::SetOwner(AActor* InActorOwner)
{
    Super::SetOwner(InActorOwner);

    AIOwner->ReceiveMoveCompleted.AddDynamic(this, &UBTT_MoveToPatrolPoint::OnMoveCompleted);

    PatrolComponent = Cast<UPatrolComponent>(
        AIOwner->GetPawn()->GetComponentByClass(UPatrolComponent::StaticClass()));
}

void UBTT_MoveToPatrolPoint::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    APatrolPathActor* Path = PatrolComponent->GetPatrolPath();

    if (GameUtils::IsValid(Path))
    {
        int Index = PatrolComponent->GetPointIndex();
        FVector Dest = PatrolComponent->GetSplinePointLocation(Index);

        OwnerController->MoveToLocation(Dest, AcceptanceRadius, true);
    }
    else
    {
        FinishExecute(false);
    }
}

void UBTT_MoveToPatrolPoint::ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    OwnerController->StopMovement();
    FinishAbort();
}

void UBTT_MoveToPatrolPoint::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    PatrolComponent->UpdatePatrolIndex();
    FinishExecute(true);
}
