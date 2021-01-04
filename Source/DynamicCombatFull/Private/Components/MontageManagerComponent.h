// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "Animation/AnimMontage.h"
#include "MontageManagerComponent.generated.h"

// This class does not need to be modified.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UMontageManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMontageManagerComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    UAnimMontage* GetMontageForAction(EMontageAction InAction, int InIndex);
    int GetMontageActionLastIndex(EMontageAction InAction) const;
    int GetRandomMontageIndex(EMontageAction InAction) const;
    EMontageAction GetLastRequestedAction() const;
    bool GetMontage(EMontageAction InAction, FMontageActionRow& OutMontageData) const;

private:
    EMontageAction LastRequestedAction;

};
