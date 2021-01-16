// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameCore/DataTypes.h"
#include "AN_UseItem.generated.h"

/**
 * 
 */
UCLASS()
class UAN_UseItem : public UAnimNotify
{
	GENERATED_BODY()
public:
    UAN_UseItem();

public:
    UFUNCTION(BlueprintCallable)
    EItemType GetItemType() const { return ItemType; }

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


private:
    UPROPERTY(EditAnywhere)
    EItemType ItemType;
};
