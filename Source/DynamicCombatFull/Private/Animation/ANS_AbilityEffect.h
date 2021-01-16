// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AbilityEffect.generated.h"

/**
 * 
 */
UCLASS()
class UANS_AbilityEffect : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
    UANS_AbilityEffect();

public:
    UFUNCTION(BlueprintCallable)
    bool IsUseInputBuffer() const { return bUseInputBuffer; }

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
    void ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin);

private:

    UPROPERTY(EditAnywhere)
    bool bUseInputBuffer;
};
