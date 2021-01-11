// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Base.h"
#include "AIController.h"
#include "BlueprintNodeHelpers.h"
#include "BehaviorTree/BehaviorTree.h"

UBTT_Base::UBTT_Base(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AIOwner != nullptr)
    {
        ReceiveExecuteAI(AIOwner, AIOwner->GetPawn());
    }

    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_Base::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AIOwner != nullptr)
    {
        ReceiveAbortAI(AIOwner, AIOwner->GetPawn());
    }

    return EBTNodeResult::Aborted;
}

void UBTT_Base::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceCreated(OwnerComp);
}

void UBTT_Base::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceDestroyed(OwnerComp);
}

void UBTT_Base::SetOwner(AActor* InActorOwner)
{
    ActorOwner = InActorOwner;
    AIOwner = Cast<AAIController>(InActorOwner);
}

void UBTT_Base::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (TickInterval.Tick(DeltaSeconds))
    {
        DeltaSeconds = TickInterval.GetElapsedTimeWithFallback(DeltaSeconds);

        if (AIOwner != nullptr)
        {
            ReceiveTickAI(AIOwner, AIOwner->GetPawn(), DeltaSeconds);
        }
        TickInterval.Reset();
    }
}

void UBTT_Base::FinishExecute(bool bSuccess)
{
    UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
    EBTNodeResult::Type NodeResult(bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);

    if (OwnerComp)
    {
        FinishLatentTask(*OwnerComp, NodeResult);
    }
}

void UBTT_Base::FinishAbort()
{
    UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
    EBTNodeResult::Type NodeResult(EBTNodeResult::Aborted);

    if (OwnerComp)
    {
        FinishLatentAbort(*OwnerComp);
    }
}
