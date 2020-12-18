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
    UPROPERTY()
    TArray<FStoredItem> Inventory;

    UPROPERTY()
    TArray<FEquipmentSlots> EquipmentSlots;

    UPROPERTY()
    bool bIsInCombat;

    UPROPERTY()
    EItemType SelectedMainHandSlotType;

    UPROPERTY()
    TMap<EStat, float> CurrentStatValues;
};
