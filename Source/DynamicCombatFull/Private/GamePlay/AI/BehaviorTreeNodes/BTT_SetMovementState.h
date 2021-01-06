// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataTypes.h"
#include "BTT_Base.h"
#include "BTT_SetMovementState.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_SetMovementState : public UBTT_Base
{
	GENERATED_BODY()
public:
    UBTT_SetMovementState(const FObjectInitializer& ObjectInitializer);

public:
    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

private:
    UPROPERTY(EditAnywhere)
        EMovementState MovementState;
};
