// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GamePlay/PatrolPathActor.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"

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

void UPatrolComponent::Init()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPathActor::StaticClass(), FoundActors);

    PatrolPath = Cast<APatrolPathActor>(UDefaultGameInstance::GetClosestActor(GetOwner(), FoundActors));
}

void UPatrolComponent::UpdatePatrolIndex()
{
    if (PatrolPath.IsValid() && PatrolPath->GetPatrolSpline() != nullptr)
    {
        USplineComponent* SplineComp = PatrolPath->GetPatrolSpline();
        if (bReverseDirection)
        {
            if (PointIndex <= 0)
            {
                if (SplineComp->IsClosedLoop())
                {
                    PointIndex = SplineComp->GetNumberOfSplinePoints() - 1;
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
            if(PointIndex >= SplineComp->GetNumberOfSplinePoints() - 1)
            {
                if (SplineComp->IsClosedLoop())
                {
                    PointIndex = 0;
                }
                else
                {
                    bReverseDirection = true;
                    PointIndex = SplineComp->GetNumberOfSplinePoints() - 2;
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
    if (PatrolPath.IsValid() && PatrolPath->GetPatrolSpline() != nullptr)
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
    return PatrolPath.IsValid();
}




