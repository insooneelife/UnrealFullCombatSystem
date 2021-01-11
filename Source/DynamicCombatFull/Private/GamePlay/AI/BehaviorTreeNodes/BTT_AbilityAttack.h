// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTT_Base.h"
#include "GameCore/DataTypes.h"
#include "BTT_AbilityAttack.generated.h"

class AAIAbilityBase;
/**
 * 
 */
UCLASS()
class UBTT_AbilityAttack : public UBTT_Base
{
	GENERATED_BODY()    

public:
    UBTT_AbilityAttack(const FObjectInitializer& ObjectInitializer);

public:
    virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
    virtual void OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp) override;
    virtual void SetOwner(AActor* InActorOwner) override;

    virtual void ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn);
    virtual void ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn);
    


    UFUNCTION()
    void OnAbilityEnded(EAbilityEndResult InResult);
    void AbilityAttack(APawn* InControlledPawn);

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AAIAbilityBase> AbilityClass;

    bool bActivateCallback;
};