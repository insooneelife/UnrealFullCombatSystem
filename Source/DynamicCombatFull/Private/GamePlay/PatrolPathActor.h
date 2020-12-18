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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    USplineComponent* GetPatrolSpline() const { return PatrolSpline; }

private:
    bool bCloseLoop;

    UPROPERTY()
    USplineComponent* PatrolSpline;

    UPROPERTY()
    UBillboardComponent* Billboard;


};
