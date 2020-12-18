// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlay/DCSSaveGame.h"
#include "DCSGameMode.generated.h"


class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameSavedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameLoadedSignature);

UCLASS()
class ADCSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    void UpdateInventoryValues();
    void UpdateEquipmentValues();
    void UpdateStatValues();

    void LoadGame();
    void SaveGame();



public:
    UPROPERTY(BlueprintAssignable)
        FGameSavedSignature OnGameSaved;

    UPROPERTY(BlueprintAssignable)
        FGameLoadedSignature OnGameLoaded;

public:
    UDCSSaveGame* GetSavedGame() const { return SavedGame; }

    FString GetSaveGameName() const { return SaveGameName; }

    const TArray<FStoredItem>& GetInventory() const { return Inventory; }

    const TArray<FEquipmentSlots>& GetEquipmentSlots() const { return EquipmentSlots; }

    bool IsInCombat() const { return bIsInCombat; }

    EItemType GetSelectedMainHandSlotType() const { return SelectedMainHandSlotType; }

    const TMap<EStat, float>& GetCurrentStatValues() const { return CurrentStatValues; }

private:
    UPROPERTY()
        UDCSSaveGame* SavedGame;

    UPROPERTY()
    FString SaveGameName;

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
