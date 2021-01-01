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

public:
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override { Super::NotifyActorBeginOverlap(OtherActor); }

    virtual bool Attach() override;
};
