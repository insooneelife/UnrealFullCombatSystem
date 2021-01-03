// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PatrolPathActor.generated.h"

class USplineComponent;
class UBillboardComponent;

UCLASS()
class APatrolPathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPathActor();

public:

    virtual void OnConstruction(const FTransform& Transform) override;

    USplineComponent* GetPatrolSpline() const { return PatrolSpline; }

private:
    UPROPERTY(EditAnywhere)
    bool bCloseLoop;

    UPROPERTY(EditAnywhere)
    USplineComponent* PatrolSpline;

    UPROPERTY(EditAnywhere)
    UBillboardComponent* Billboard;


};
