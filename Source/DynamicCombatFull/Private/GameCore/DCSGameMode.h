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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

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
    UPROPERTY(EditAnywhere)
        UDCSSaveGame* SavedGame;

    UPROPERTY(EditAnywhere)
    FString SaveGameName;

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
