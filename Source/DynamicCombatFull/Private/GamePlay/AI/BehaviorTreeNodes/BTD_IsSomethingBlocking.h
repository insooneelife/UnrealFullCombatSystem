// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameCore/DataTypes.h"
#include "BTD_Base.h"
#include "BTD_IsSomethingBlocking.generated.h"

/**
 * 
 */
UCLASS()
class UBTD_IsSomethingBlocking : public UBTD_Base
{
	GENERATED_BODY()
	
public:
    UBTD_IsSomethingBlocking();

protected:
    virtual bool PerformConditionCheckAI(
        UBehaviorTreeComponent& InOwnerComp,
        AAIController* InOwnerController,
        APawn* InControlledPawn) const override;

private:
    UPROPERTY(EditAnywhere)
    EDirection Direction;

    UPROPERTY(EditAnywhere)
    float Distance;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<ETraceTypeQuery> TraceChannel;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EDrawDebugTrace::Type> DebugType;
};
