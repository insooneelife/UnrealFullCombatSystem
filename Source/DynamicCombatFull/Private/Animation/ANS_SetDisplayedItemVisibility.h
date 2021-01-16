// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameCore/DataTypes.h"
#include "ANS_SetDisplayedItemVisibility.generated.h"

class UEquipmentComponent;
class USkeletalMeshComponent;
/**
 * 
 */
UCLASS()
class UANS_SetDisplayedItemVisibility : public UAnimNotifyState
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    EItemType GetItemType() const { return ItemType; }

    UFUNCTION(BlueprintCallable)
    bool HasToBeInCombat() const { return bHasToBeInCombat; }

    UFUNCTION(BlueprintCallable)
    bool HideThenShow() const { return bHideThenShow; }

public:
    UANS_SetDisplayedItemVisibility();

public:
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
    void ExecuteNotify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, bool bBegin);
    void SetItemVisibility(UEquipmentComponent* EquipmentComponent, bool bVisible);

private:
    UPROPERTY(EditAnywhere)
    EItemType ItemType;
    
    UPROPERTY(EditAnywhere)
    bool bHasToBeInCombat;

    UPROPERTY(EditAnywhere)
    bool bHideThenShow;

};
