// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementSpeedComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UMovementSpeedComponent::UMovementSpeedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    MovementState = EMovementState::Idle;
    StartMovementState = EMovementState::Jog;
    WalkSpeed = 200.0f;
    JogSpeed = 400.0f;
    SprintSpeed = 550.0f;
    SpeedChangeInterpSpeed = 3.0f;
    StatesToToggle = { EMovementState::Walk, EMovementState::Jog };
}


// Called when the game starts
void UMovementSpeedComponent::BeginPlay()
{
	Super::BeginPlay();

    MovementState = StartMovementState;
    Movement = Cast<UCharacterMovementComponent>(
        GetOwner()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));

    SetMovementState(MovementState);
    if (!GameUtils::IsValid(Movement))
    {
        UE_LOG(LogTemp, Error, TEXT("Movement is not valid!!"));
    }
}


// Called every frame
void UMovementSpeedComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateMaxSpeed();
}


EMovementState UMovementSpeedComponent::GetMovementState() const
{
    return MovementState;
}

void UMovementSpeedComponent::SetMovementState(EMovementState State)
{
    if (GameUtils::IsValid(Movement))
    {
        OnMovementStateEnd.Broadcast(MovementState);
        MovementState = StartMovementState;


        if (MovementState == EMovementState::Idle)
        {
            TargetSpeed = 0.0f;
        }
        else if (MovementState == EMovementState::Walk)
        {
            TargetSpeed = WalkSpeed;
        }
        else if (MovementState == EMovementState::Jog)
        {
            TargetSpeed = JogSpeed;
        }
        else if (MovementState == EMovementState::Sprint)
        {
            TargetSpeed = SprintSpeed;
        }

        bIsUpdatingSpeed = true;

        OnMovementStateStart.Broadcast(MovementState);
    }
}

void UMovementSpeedComponent::UpdateMaxSpeed()
{
    if (bIsUpdatingSpeed)
    {
        if (GameUtils::IsValid(Movement))
        {
            Movement->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(
                Movement->MaxWalkSpeed, 
                TargetSpeed, 
                UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 
                SpeedChangeInterpSpeed);

            if (UKismetMathLibrary::NearlyEqual_FloatFloat(Movement->MaxWalkSpeed, TargetSpeed))
            {
                UE_LOG(LogTemp, Error, TEXT("UpdateMaxSpeed  %f"), TargetSpeed);
                Movement->MaxWalkSpeed = TargetSpeed;

                bIsUpdatingSpeed = false;
            }
        }
    }
}

void UMovementSpeedComponent::ToggleState()
{
    int StateIndex = StatesToToggle.Find(MovementState);

    if (StateIndex >= 0)
    {
        int LastIndex = StatesToToggle.Num() - 1;
        EMovementState ChangeState;

        if (StateIndex + 1 >= LastIndex)
        {
            ChangeState = StatesToToggle[0];
        }
        else
        {
            ChangeState = StatesToToggle[LastIndex];
        }

        SetMovementState(ChangeState);
    }
}

float UMovementSpeedComponent::GetMaxPossibleSpeed() const
{
    if (GameUtils::IsValid(Movement))
    {
        return Movement->MaxWalkSpeed;
    }

    return 0;
}

