// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Base.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_Base : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
    virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp) override;

    virtual void SetOwner(AActor* InActorOwner) override;

protected:
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn) {}
    virtual void ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn) {}
    virtual void ReceiveTickAI(AAIController* OwnerController, APawn* ControlledPawn, float DeltaSeconds) {}

    virtual void FinishExecute(bool bSuccess);
    virtual void FinishAbort();


protected:
    TWeakObjectPtr<AAIController> AIOwner;

    TWeakObjectPtr<AActor> ActorOwner;

    /** If any of the Tick functions is implemented, how ofter should they be ticked.
     *	Values < 0 mean 'every tick'. */
    UPROPERTY(EditAnywhere, Category = Task)
    FIntervalCountdown TickInterval;
};
