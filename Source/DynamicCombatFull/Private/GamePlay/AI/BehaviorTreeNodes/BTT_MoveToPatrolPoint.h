// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameCore/DataTypes.h"
#include "BTT_Base.h"
#include "BTT_MoveToPatrolPoint.generated.h"

class UPatrolComponent;
/**
 * 
 */
UCLASS()
class UBTT_MoveToPatrolPoint : public UBTT_Base
{
	GENERATED_BODY()
public:
    UBTT_MoveToPatrolPoint(const FObjectInitializer& ObjectInitializer);

public:
    virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
    virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp) override;
    virtual void SetOwner(AActor* InActorOwner) override;

    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);
    virtual void ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn);
    
    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
    UPROPERTY()
    UPatrolComponent* PatrolComponent;

    UPROPERTY(EditAnywhere)
    float AcceptanceRadius;
};
