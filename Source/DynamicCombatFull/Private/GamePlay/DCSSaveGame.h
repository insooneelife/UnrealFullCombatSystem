// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "DCSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UDCSSaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
    UPROPERTY(EditAnywhere)
    TArray<FStoredItem> Inventory;

    UPROPERTY(EditAnywhere)
    TArray<FEquipmentSlots> EquipmentSlots;

    UPROPERTY(EditAnywhere)
    bool bIsInCombat;

    UPROPERTY(EditAnywhere)
    EItemType SelectedMainHandSlotType;

    UPROPERTY(EditAnywhere)
    TMap<EStat, float> CurrentStatValues;
};
