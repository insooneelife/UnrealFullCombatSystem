// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTS_Base.h"
#include "GameCore/DataTypes.h"
#include "BTS_UpdateMeleeAIBehavior.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class UBTS_UpdateMeleeAIBehavior : public UBTS_Base
{
	GENERATED_BODY()

public:
    UBTS_UpdateMeleeAIBehavior(const FObjectInitializer& ObjectInitializer);

    virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
    virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp) override;
    virtual void SetOwner(AActor* InActorOwner) override;

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
    TWeakObjectPtr<AAICharacter> ControlledCharacter;

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
