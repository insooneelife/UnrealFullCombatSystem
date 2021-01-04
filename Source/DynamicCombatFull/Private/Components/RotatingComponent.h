// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "RotatingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotatingStartSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotatingEndSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URotatingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URotatingComponent();

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable)
    void StartRotatingWithLimit(float InMaxPossibleRotation, float InMaxDegreesPerSecond);

    UFUNCTION(BlueprintCallable)
    void StartRotatingWithTime(float InTime, float InMaxDegreesPerSecond);
    
    UFUNCTION(BlueprintCallable)
    void StopRotating();

    UFUNCTION(BlueprintCallable)
    void DebugDraw() const;

public:
    void SetRotationMode(ERotationMode InMode);

private:
    bool IsRotating() const { return bShouldRotate; }

public:
    UPROPERTY(BlueprintAssignable)
    FRotatingStartSignature OnRotatingStart;

    UPROPERTY(BlueprintAssignable)
    FRotatingEndSignature OnRotatingEnd;

private:
    float TimeElapsed;
    float RotateTime;
    float MaxDegreesPerSecond;
    bool bShouldRotate;
    float MaxAllowedDegrees;

};
