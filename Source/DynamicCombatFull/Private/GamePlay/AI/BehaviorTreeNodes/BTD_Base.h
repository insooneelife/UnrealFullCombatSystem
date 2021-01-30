// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_Base.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UBTD_Base : public UBTDecorator
{
	GENERATED_BODY()
	
public:

    UBTD_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool CalculateRawConditionValue(
        UBehaviorTreeComponent& InOwnerComp, uint8* InNodeMemory) const override final;

    virtual void SetOwner(AActor* InActorOwner) override;

protected:
    virtual bool PerformConditionCheckAI(
        UBehaviorTreeComponent& InOwnerComp, 
        AAIController* InOwnerController, 
        APawn* InControlledPawn) const { return true; }

protected:
    TWeakObjectPtr<AAIController> AIOwner;
    TWeakObjectPtr<AActor> ActorOwner;
};
