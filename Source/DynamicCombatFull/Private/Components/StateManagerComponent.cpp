// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    CurrentState = EState::Idle;
}

void UStateManagerComponent::SetActivity(EActivity InActivity, bool bInValue)
{
    if (GetActivityValue(InActivity) != bInValue)
    {
        Activities.Add(InActivity, bInValue);

        OnActivityChanged.Broadcast(InActivity, bInValue);
    }
}

void UStateManagerComponent::SetState(EState InState)
{
    GetWorld()->GetTimerManager().ClearTimer(SetIdleStateTimerHandle);
    EState PrevState = CurrentState;
    CurrentState = InState;

    if (PrevState != CurrentState)
    {
        OnStateChanged.Broadcast(PrevState, CurrentState);
    }
}

bool UStateManagerComponent::GetActivityValue(EActivity InActivity)
{
    if (Activities.Contains(InActivity))
    {
        return Activities[InActivity];
    }
    return false;
}

void UStateManagerComponent::ResetState(float InTime)
{
    if (GetState() != EState::Dead)
    {
        if (InTime <= 0.0f)
        {
            SetIdleState();
        }
        else
        {
            GetWorld()->GetTimerManager().SetTimer(
                SetIdleStateTimerHandle, this, &UStateManagerComponent::SetIdleState, InTime, false);
        }
    }
}


