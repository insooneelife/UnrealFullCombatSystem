// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "StateManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateChangedSignature, EState, PrevState, EState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActivityChangedSignature, EActivity, Activity, bool, Value);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
    void SetState(EState State);

    EState GetState() const;

    UFUNCTION(BlueprintCallable)
    void SetActivity(EActivity Activity, bool Value);

    bool GetActivityValue(EActivity Activity);

    void ResetState(float InTime);

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
