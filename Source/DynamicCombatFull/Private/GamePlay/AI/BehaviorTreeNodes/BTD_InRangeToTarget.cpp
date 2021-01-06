// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_InRangeToTarget.h"

#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTD_InRangeToTarget::UBTD_InRangeToTarget()
    :
    LessThan(300.0f)
{ 
}

bool UBTD_InRangeToTarget::PerformConditionCheckAI(
    UBehaviorTreeComponent& InOwnerComp,
    AAIController* InOwnerController,
    APawn* InControlledPawn) const
{
    AActor* Target = Cast<AActor>(InOwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
    float Value = InControlledPawn->GetDistanceTo(Target);

    return UKismetMathLibrary::InRange_FloatFloat(Value, GreaterThan, LessThan);
}