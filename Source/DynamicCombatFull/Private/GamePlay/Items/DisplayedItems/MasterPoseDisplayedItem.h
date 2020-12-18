// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "MasterPoseDisplayedItem.generated.h"

/**
 * 
 */
UCLASS()
class AMasterPoseDisplayedItem : public ADisplayedItem
{
	GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor);

    // Called every frame
    virtual void Tick(float DeltaTime) override;


    virtual bool Attach() override;
};
