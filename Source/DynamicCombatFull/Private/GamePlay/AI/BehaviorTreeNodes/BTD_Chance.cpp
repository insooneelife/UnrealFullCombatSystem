// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_Chance.h"

bool UBTD_Chance::PerformConditionCheckAI(
    UBehaviorTreeComponent& InOwnerComp,
    AAIController* InOwnerController,
    APawn* InControlledPawn) const
{
    return FMath::RandRange(1, 100) <= Chance;
}