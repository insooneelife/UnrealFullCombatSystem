// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameCore/DataTypes.h"
#include "ANS_Activity.generated.h"

/**
 * 
 */
UCLASS()
class UANS_Activity : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    UANS_Activity();

public:
    UFUNCTION(BlueprintCallable)
    EActivity GetActivity() const { return Activity; }

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
    void ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin);

private:
    UPROPERTY(EditAnywhere)
    EActivity Activity;
};
