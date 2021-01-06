// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataTypes.h"
#include "BTT_Base.h"
#include "BTT_PlayMontageByAction.generated.h"

/**
 * 
 */
UCLASS()
class UBTT_PlayMontageByAction : public UBTT_Base
{
	GENERATED_BODY()
public:
    UBTT_PlayMontageByAction(const FObjectInitializer& ObjectInitializer);

public:
    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);
    void Delayed_Execute();

private:
    UPROPERTY(EditAnywhere)
    EMontageAction Action;

    UPROPERTY(EditAnywhere)
    int Index;

    UPROPERTY(EditAnywhere)
    float PlayRate;

    UPROPERTY(EditAnywhere)
    bool bUseRandomIndex;

    FTimerHandle RetriggerableDelayTimerHandle;
};
