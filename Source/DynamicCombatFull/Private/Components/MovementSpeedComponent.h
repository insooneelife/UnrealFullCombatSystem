// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "MovementSpeedComponent.generated.h"

class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementStateStartSignature, EMovementState, InState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMovementStateEndSignature, EMovementState, InState);

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
    UFUNCTION(BlueprintCallable)
    float GetWalkSpeed() const { return WalkSpeed; }

    UFUNCTION(BlueprintCallable)
    float GetJogSpeed() const { return JogSpeed; }

    UFUNCTION(BlueprintCallable)
    float GetSprintSpeed() const { return SprintSpeed; }

public:
    void ToggleState();
    void SetMovementState(EMovementState InState);
    EMovementState GetMovementState() const { return MovementState; }

    void SetWalkSpeed(float InWalkSpeed) { WalkSpeed = InWalkSpeed; }
    void SetJogSpeed(float InJogSpeed) { JogSpeed = InJogSpeed; }
    void SetSprintSpeed(float InSprintSpeed) { SprintSpeed = InSprintSpeed; }

private:
    void UpdateMaxSpeed();
    float GetMaxPossibleSpeed() const;

public:

    UPROPERTY(BlueprintAssignable)
    FMovementStateStartSignature OnMovementStateStart;
    
    UPROPERTY(BlueprintAssignable)
    FMovementStateEndSignature OnMovementStateEnd;

private:
    UPROPERTY()
    UCharacterMovementComponent* Movement;

    UPROPERTY(EditAnywhere)
    EMovementState StartMovementState;

    UPROPERTY(EditAnywhere)
    float WalkSpeed;

    UPROPERTY(EditAnywhere)
    float JogSpeed;

    UPROPERTY(EditAnywhere)
    float SprintSpeed;

    UPROPERTY(EditAnywhere)
    float SpeedChangeInterpSpeed;

    UPROPERTY(EditAnywhere)
    TArray<EMovementState> StatesToToggle;

    EMovementState MovementState;
    float TargetSpeed;
    bool bIsUpdatingSpeed;
		
};
