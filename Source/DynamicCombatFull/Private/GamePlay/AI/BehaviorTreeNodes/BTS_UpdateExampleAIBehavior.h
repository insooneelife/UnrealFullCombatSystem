// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTS_Base.h"
#include "GameCore/DataTypes.h"
#include "BTS_UpdateExampleAIBehavior.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class UBTS_UpdateExampleAIBehavior : public UBTS_Base
{
	GENERATED_BODY()

public:
    UBTS_UpdateExampleAIBehavior(const FObjectInitializer& ObjectInitializer);

    virtual void ReceiveTickAI(
        UBehaviorTreeComponent& OwnerBTree,
        AAIController* InOwnerController,
        APawn* InControlledPawn,
        float InDeltaSeconds) override;

    virtual void ReceiveSearchStartAI(
        UBehaviorTreeComponent& OwnerBTree,
        AAIController* InOwnerController,
        APawn* InControlledPawn) override;

    UFUNCTION()
    void OnStateChanged(EState InPrevState, EState InNewState);

    UFUNCTION()
    void OnStaminaValueChanged(float InNewValue, float InMaxValue);

    void Delayed_Set();

    void Update();

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
    float AttackBehaviorRange;

    float TicksNearTarget;

    bool bIsOutOfStamina;

    FTimerHandle RetriggerableDelayTimerHandle;

};
