// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataTypes.h"
#include "BTT_Base.h"
#include "BTT_BowAttack.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_BowAttack : public UBTT_Base
{
	GENERATED_BODY()
	
public:
    UBTT_BowAttack(const FObjectInitializer& ObjectInitializer);

public:
    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

    void Delayed_Execute();
};
