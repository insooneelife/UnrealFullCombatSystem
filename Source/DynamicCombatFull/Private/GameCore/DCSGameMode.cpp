// Fill out your copyright notice in the Description page of Project Settings.


#include "DCSGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Components/InventoryComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/EquipmentComponent.h"

// Called when the game starts
void ADCSGameMode::BeginPlay()
{
    Super::BeginPlay();

    UpdateInventoryValues();
    UpdateEquipmentValues();
    UpdateStatValues();

}

void ADCSGameMode::UpdateInventoryValues()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController->IsValidLowLevel())
    {
        UInventoryComponent* InventoryComp = 
            Cast<UInventoryComponent>(PlayerController->GetComponentByClass(UInventoryComponent::StaticClass()));

        if (InventoryComp->IsValidLowLevel())
        {
            Inventory = InventoryComp->GetInventory();
        }
    }
}

void ADCSGameMode::UpdateEquipmentValues()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController->IsValidLowLevel())
    {
        UEquipmentComponent* EquipComp =
            Cast<UEquipmentComponent>(PlayerController->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (EquipComp->IsValidLowLevel())
        {
            EquipmentSlots = EquipComp->GetEquipmentSlots();
            bIsInCombat = EquipComp->IsInCombat();
            SelectedMainHandSlotType = EquipComp->GetSelectedMainHandType();
        }
    }
}

void ADCSGameMode::UpdateStatValues()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController->IsValidLowLevel())
    {
        TArray<UActorComponent*> StatComps;
        PlayerController->GetComponents(UExtendedStatComponent::StaticClass(), StatComps);

        for (UActorComponent* Comp : StatComps)
        {
            UExtendedStatComponent* StatComp = Cast<UExtendedStatComponent>(Comp);

            if (StatComp->IsValidLowLevel())
            {
                CurrentStatValues.Add(StatComp->GetStatType(), StatComp->GetCurrentValue());
            }
        }
    }
}


void ADCSGameMode::LoadGame()
{
    SaveGameName;

    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameName, 0);

    SavedGame = Cast<UDCSSaveGame>(LoadedGame);

    if (SavedGame == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Load Failed!"));
    }
    else
    {
        EquipmentSlots = SavedGame->EquipmentSlots;
        Inventory = SavedGame->Inventory;
        bIsInCombat = SavedGame->bIsInCombat;
        SelectedMainHandSlotType = SavedGame->SelectedMainHandSlotType;
        CurrentStatValues = SavedGame->CurrentStatValues;

        OnGameLoaded.Broadcast();

        UE_LOG(LogTemp, Warning, TEXT("Game Loaded!"));
    }
}

void ADCSGameMode::SaveGame()
{
    if (SavedGame->IsValidLowLevel())
    {
        UpdateInventoryValues();
        UpdateEquipmentValues();
        UpdateStatValues();

        SavedGame->EquipmentSlots = EquipmentSlots;
        SavedGame->Inventory = Inventory;
        SavedGame->bIsInCombat = bIsInCombat;
        SavedGame->SelectedMainHandSlotType = SelectedMainHandSlotType;
        SavedGame->CurrentStatValues = CurrentStatValues;

        UGameplayStatics::SaveGameToSlot(SavedGame, SaveGameName, 0);

        OnGameSaved.Broadcast();

        UE_LOG(LogTemp, Warning, TEXT("Game Saved!"));
    }
    else
    {
        SavedGame = Cast<UDCSSaveGame>(UGameplayStatics::CreateSaveGameObject(UDCSSaveGame::StaticClass()));
    }
}