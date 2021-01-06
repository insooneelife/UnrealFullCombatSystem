// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_Base.h"
#include "BTD_CanShoot.generated.h"

/**
 * 
 */
UCLASS()
class UBTD_CanShoot : public UBTD_Base
{
	GENERATED_BODY()
	
public:

protected:
    virtual bool PerformConditionCheckAI(
        UBehaviorTreeComponent& InOwnerComp, 
        AAIController* InOwnerController, 
        APawn* InControlledPawn) const override;

private:
    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector TargetKey;
};
