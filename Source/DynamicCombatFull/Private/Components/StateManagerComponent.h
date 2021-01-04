// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "StateManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateChangedSignature, EState, InPrevState, EState, InNewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActivityChangedSignature, EActivity, InActivity, bool, bInValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();

public:
    UFUNCTION(BlueprintCallable)
    void SetActivity(EActivity InActivity, bool bInValue);

public:
    void SetState(EState InState);
    bool GetActivityValue(EActivity InActivity);
    void ResetState(float InTime);
    EState GetState() const { return CurrentState; }

private:
    void SetIdleState() { SetState(EState::Idle); }

public:
    UPROPERTY(BlueprintAssignable)
    FStateChangedSignature OnStateChanged;

    UPROPERTY(BlueprintAssignable)
    FActivityChangedSignature OnActivityChanged;

private:
    EState CurrentState;
    TMap<EActivity, bool> Activities;
    FTimerHandle SetIdleStateTimerHandle;
};
