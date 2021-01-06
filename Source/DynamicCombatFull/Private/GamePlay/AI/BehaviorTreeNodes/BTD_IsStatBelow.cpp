// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsStatBelow.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"
#include "Components/ExtendedStatComponent.h"

UBTD_IsStatBelow::UBTD_IsStatBelow()
    :
    Stat(EStat::Health)
{
}

bool UBTD_IsStatBelow::PerformConditionCheckAI(
    UBehaviorTreeComponent& InOwnerComp,
    AAIController* InOwnerController,
    APawn* InControlledPawn) const
{
    UExtendedStatComponent* StatComponent =
        UDefaultGameInstance::GetExtendedStatComponent(InControlledPawn, Stat);

    if (GameUtils::IsValid(StatComponent))
    {
        float Value = (StatComponent->GetCurrentValue() / StatComponent->GetMaxValue()) * 100.0f;
        return Value < Percent;
    }
    else
    {
        return true;
    }
}