// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotateOwner.generated.h"

/**
 * 
 */
UCLASS()
class UANS_RotateOwner : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetMaxDegreesPerSecond() const { return MaxDegreesPerSecond; }

public:
    UANS_RotateOwner();

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
    void ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin);

private:
    UPROPERTY(EditAnywhere)
    float MaxDegreesPerSecond;
};
