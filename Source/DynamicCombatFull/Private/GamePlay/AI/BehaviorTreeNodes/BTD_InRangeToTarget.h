// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_Base.h"
#include "BTD_InRangeToTarget.generated.h"

/**
 * 
 */
UCLASS()
class UBTD_InRangeToTarget : public UBTD_Base
{
	GENERATED_BODY()
	
public:
    UBTD_InRangeToTarget();

protected:
    virtual bool PerformConditionCheckAI(
        UBehaviorTreeComponent& InOwnerComp,
        AAIController* InOwnerController,
        APawn* InControlledPawn) const override;

private:
    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere)
    float GreaterThan;

    UPROPERTY(EditAnywhere)
    float LessThan;
};
