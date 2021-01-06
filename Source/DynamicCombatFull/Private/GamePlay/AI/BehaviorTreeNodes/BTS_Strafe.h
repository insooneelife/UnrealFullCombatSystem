// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTS_Base.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "BTS_Strafe.generated.h"


class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
/**
 * 
 */
UCLASS()
class UBTS_Strafe : public UBTS_Base
{
	GENERATED_BODY()
	
public:
    UBTS_Strafe(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void ReceiveTickAI(
        UBehaviorTreeComponent& OwnerBTree,
        AAIController* InOwnerController,
        APawn* InControlledPawn,
        float InDeltaSeconds) override;

    virtual void ReceiveActivationAI(
        UBehaviorTreeComponent& OwnerBTree, 
        AAIController* InOwnerController,
        APawn* InControlledPawn) override;

    virtual void ReceiveDeactivationAI(
        UBehaviorTreeComponent& OwnerBTree,
        AAIController* InOwnerController,
        APawn* InControlledPawn) override;

    void Strafe(const UBehaviorTreeComponent& OwnerBTree);

    UEnvQuery* GetStrafeQuery(const UBehaviorTreeComponent& OwnerBTree) const;

    UFUNCTION()
    void OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

private:

    UPROPERTY()
    APawn* ControlledPawn;

    UPROPERTY()
    AAIController* OwnerController;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UEnvQuery* EQS_StrafeLeftObject;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UEnvQuery* EQS_StrafeRightObject;

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere)
    bool bUseRandomDirection;

};
