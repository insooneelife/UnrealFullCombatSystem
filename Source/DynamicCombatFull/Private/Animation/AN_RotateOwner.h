// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_RotateOwner.generated.h"

/**
 * 
 */
UCLASS()
class UAN_RotateOwner : public UAnimNotify
{
	GENERATED_BODY()
public:
    UAN_RotateOwner();

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

    UFUNCTION(BlueprintCallable)
        float GetMaxDegreesPerSecond() const { return MaxDegreesPerSecond; }

    UFUNCTION(BlueprintCallable)
        float GetMaxPossibleRotation() const { return MaxPossibleRotation; }

private:
    UPROPERTY(EditAnywhere)
    float MaxDegreesPerSecond;

    UPROPERTY(EditAnywhere)
    float MaxPossibleRotation;
};
