// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameCore/CustomStructs.h"
#include "ANS_HitBox.generated.h"

/**
 * 
 */
UCLASS()
class UANS_HitBox : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    ECollisionPart GetCollisionPart() const { return CollisionPart; }

public:
    UANS_HitBox();

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
    void ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin);

private:
    UPROPERTY(EditAnywhere)
    ECollisionPart CollisionPart;
};
