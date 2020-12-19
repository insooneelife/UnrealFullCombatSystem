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


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UStateManagerComponent::SetState(EState State)
{
    GetWorld()->GetTimerManager().ClearTimer(SetIdleStateTimerHandle);
    EState PrevState = CurrentState;
    CurrentState = State;

    if (PrevState != CurrentState)
    {
        OnStateChanged.Broadcast(PrevState, CurrentState);
    }

}

EState UStateManagerComponent::GetState() const
{
    return CurrentState;
}

void UStateManagerComponent::SetActivity(EActivity Activity, bool Value)
{
    if (GetActivityValue(Activity) != Value)
    {
        Activities.Add(Activity, Value);

        OnActivityChanged.Broadcast(Activity, Value);
    }
}

bool UStateManagerComponent::GetActivityValue(EActivity Activity)
{
    if (Activities.Contains(Activity))
    {
        return true;
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


