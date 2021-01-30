// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_Base.h"
#include "AIController.h"

UBTD_Base::UBTD_Base(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNodeInstance = true;
}

bool UBTD_Base::CalculateRawConditionValue(UBehaviorTreeComponent& InOwnerComp, uint8* InNodeMemory) const
{
    if (AIOwner.IsValid())
    {
        bool CurrentCallResult = PerformConditionCheckAI(InOwnerComp, AIOwner.Get(), AIOwner->GetPawn());
        return CurrentCallResult;
    }
    return false;
}

void UBTD_Base::SetOwner(AActor* InActorOwner)
{
    ActorOwner = InActorOwner;
    AIOwner = Cast<AAIController>(InActorOwner);
}