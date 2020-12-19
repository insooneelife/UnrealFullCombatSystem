// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolComponent.h"
#include "GamePlay/PatrolPathActor.h"

// Sets default values for this component's properties
UPatrolComponent::UPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}


// Called when the game starts
void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPatrolComponent::UpdatePatrolIndex()
{
    if (PatrolPath->IsValidLowLevel())
    {
        if (bReverseDirection)
        {
            if (PointIndex <= 0)
            {
                if (PatrolPath->GetPatrolSpline()->IsClosedLoop())
                {
                    PointIndex = PatrolPath->GetPatrolSpline()->GetNumberOfSplinePoints() - 1;
                }
                else
                {
                    PointIndex = 1;
                    bReverseDirection = false;
                }
            }
            else
            {
                PointIndex--;
            }
        }
        else
        {
            if(PointIndex >= PatrolPath->GetPatrolSpline()->GetNumberOfSplinePoints() - 1)
            {
                if (PatrolPath->GetPatrolSpline()->IsClosedLoop())
                {
                    PointIndex = 0;
                }
                else
                {
                    bReverseDirection = true;
                    PointIndex = PatrolPath->GetPatrolSpline()->GetNumberOfSplinePoints() - 2;
                }
            }
            else
            {
                PointIndex++;
            }
        }
    }
}

FVector UPatrolComponent::GetSplinePointLocation(int InPointIndex) const
{
    if (PatrolPath->IsValidLowLevel())
    {
        return PatrolPath->GetPatrolSpline()->GetLocationAtSplinePoint(InPointIndex, ESplineCoordinateSpace::World);
    }
    else
    {
        return FVector::ZeroVector;
    }
}

bool UPatrolComponent::IsPatrolPathValid() const
{
    return PatrolPath->IsValidLowLevel();
}




