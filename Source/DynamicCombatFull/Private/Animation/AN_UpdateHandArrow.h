// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_UpdateHandArrow.generated.h"

/**
 * 
 */
UCLASS()
class UAN_UpdateHandArrow : public UAnimNotify
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    bool IsVisible() const { return bVisible; }

    UFUNCTION(BlueprintCallable)
    bool IsAttachToHand() const { return bAttachToHand; }

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
    UPROPERTY(EditAnywhere)
    bool bVisible;

    UPROPERTY(EditAnywhere)
    bool bAttachToHand;
};
