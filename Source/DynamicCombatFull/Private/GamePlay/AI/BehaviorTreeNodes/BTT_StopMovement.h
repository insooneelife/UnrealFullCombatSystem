// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataTypes.h"
#include "BTT_Base.h"
#include "BTT_StopMovement.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_StopMovement : public UBTT_Base
{
	GENERATED_BODY()
public:
    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

};
