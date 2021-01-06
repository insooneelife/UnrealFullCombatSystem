// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameCore/DataTypes.h"
#include "BTD_Base.h"
#include "BTD_IsStatBelow.generated.h"

/**
 * 
 */
UCLASS()
class UBTD_IsStatBelow : public UBTD_Base
{
	GENERATED_BODY()
	
public:
    UBTD_IsStatBelow();

protected:
    virtual bool PerformConditionCheckAI(
        UBehaviorTreeComponent& InOwnerComp,
        AAIController* InOwnerController,
        APawn* InControlledPawn) const override;

private:
    UPROPERTY(EditAnywhere)
    EStat Stat;

    UPROPERTY(EditAnywhere)
    float Percent;
};
