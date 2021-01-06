// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTS_Base.h"
#include "GameCore/DataTypes.h"
#include "BTS_UpdateArcherAIBehavior.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class UBTS_UpdateArcherAIBehavior : public UBTS_Base
{
	GENERATED_BODY()
public:
    UBTS_UpdateArcherAIBehavior(const FObjectInitializer& ObjectInitializer);

protected:

    virtual void ReceiveTickAI(
        UBehaviorTreeComponent& OwnerBTree, 
        AAIController* InOwnerController,
        APawn* InControlledPawn,
        float InDeltaSeconds) override;

    virtual void ReceiveSearchStartAI(
        UBehaviorTreeComponent& OwnerBTree,
        AAIController* InOwnerController,
        APawn* InControlledPawn) override;

    void OnStateChanged(EState InPrevState, EState InNewState);

    void Update();
    void Flee();

    void Delayed_Flee();

    void UpdateBehavior();
    void UpdateActivities();
    void SetBehavior(EAIBehavior InBehavior);

private:
    UPROPERTY()
        AAIController* OwnerController;

    UPROPERTY()
        AAICharacter* ControlledCharacter;

    UPROPERTY(EditAnywhere)
        FBlackboardKeySelector BehaviorKey;

    UPROPERTY(EditAnywhere)
        FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere)
    float FleeBehaviorRange;

    bool bIsFleeing;

    FTimerHandle RetriggerableDelayTimerHandle;
};
