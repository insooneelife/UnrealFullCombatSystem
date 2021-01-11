// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/AI/AICharacter.h"
#include "MeleeAICharacter.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class AMeleeAICharacter : public AAICharacter
{
	GENERATED_BODY()
public:
    // Sets default values for this character's properties
    AMeleeAICharacter();

public:
    // IMontageManagerInterface
    virtual UDataTable* GetMontages(EMontageAction InAction) const override;


private:
    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UDataTable* AIMeleeMontages;
};
