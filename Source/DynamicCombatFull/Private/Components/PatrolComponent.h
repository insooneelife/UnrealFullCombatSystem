// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PatrolComponent.generated.h"

class APatrolPathActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
    void Init();
    void UpdatePatrolIndex();
    FVector GetSplinePointLocation(int InPointIndex) const;
    bool IsPatrolPathValid() const;

public:
    APatrolPathActor* GetPatrolPath() const { return PatrolPath.Get(); }
    int GetPointIndex() const { return PointIndex; }
    bool IsReverseDirection() const { return bReverseDirection; }

private:
    TWeakObjectPtr<APatrolPathActor> PatrolPath;
    int PointIndex;
    bool bReverseDirection;

};
