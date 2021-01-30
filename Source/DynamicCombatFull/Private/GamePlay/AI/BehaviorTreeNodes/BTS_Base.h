// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Base.generated.h"

class AActor;
class AAIController;
class APawn;
class UBehaviorTree;

UCLASS(Abstract)
class UBTS_Base : public UBTService
{
    GENERATED_UCLASS_BODY()

    virtual void SetOwner(AActor* ActorOwner) override;

    /** called when node instance is added to tree */
    virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override ;

    /** called when node instance is removed from tree */
    virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp) override;

protected:

    TWeakObjectPtr<AAIController> AIOwner;
    TWeakObjectPtr<AActor> ActorOwner;

    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

    virtual void ReceiveTickAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn, float InDeltaSeconds) {}
    virtual void ReceiveSearchStartAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn) {}
    virtual void ReceiveActivationAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn) {}
    virtual void ReceiveDeactivationAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn) {}
    virtual bool IsServiceActive() const;
};
