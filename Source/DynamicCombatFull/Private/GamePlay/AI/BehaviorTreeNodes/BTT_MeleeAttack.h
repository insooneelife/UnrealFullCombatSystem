// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataTypes.h"
#include "BTT_Base.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_MeleeAttack : public UBTT_Base
{
	GENERATED_BODY()
public:
    UBTT_MeleeAttack(const FObjectInitializer& ObjectInitializer);

public:
    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);

    void Delayed_Execute();

private:
    UPROPERTY(EditAnywhere)
    EMeleeAttackType AttackType;
};
