// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_Base.h"
#include "BTD_Chance.generated.h"

/**
 * 
 */
UCLASS()
class UBTD_Chance : public UBTD_Base
{
	GENERATED_BODY()

protected:
    virtual bool PerformConditionCheckAI(
        UBehaviorTreeComponent& InOwnerComp,
        AAIController* InOwnerController,
        APawn* InControlledPawn) const override;

private:
    UPROPERTY(EditAnywhere)
    int Chance;
};
