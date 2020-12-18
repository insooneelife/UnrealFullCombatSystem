// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "MovementSpeedComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementStateStartSignature, EMovementState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementStateEndSignature, EMovementState, State);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMovementSpeedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementSpeedComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

    EMovementState GetMovementState() const;
    void SetMovementState(EMovementState State);

    void UpdateMaxSpeed();
    void ToggleState();
    float GetMaxPossibleSpeed() const;

public:

    UPROPERTY(BlueprintAssignable)
    FMovementStateStartSignature OnMovementStateStart;
    
    UPROPERTY(BlueprintAssignable)
    FMovementStateEndSignature OnMovementStateEnd;

private:
    EMovementState MovementState;
    EMovementState StartMovementState;
    float WalkSpeed;
    float JogSpeed;
    float SprintSpeed;
    float TargetSpeed;
    float SpeedChangeInterpSpeed;
    TArray<EMovementState> StatesToToggle;
    bool bIsUpdatingSpeed;
		
};
