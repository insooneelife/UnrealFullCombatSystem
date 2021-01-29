// Fill out your copyright notice in the Description page of Project Settings.


#include "DCSGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/InventoryComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/GameUtils.h"

// Called when the game starts
void ADCSGameMode::BeginPlay()
{
    Super::BeginPlay();

    UpdateInventoryValues();
    UpdateEquipmentValues();
    UpdateStatValues();
}

void ADCSGameMode::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    Super::EndPlay(EndPlayResult);

    SavedGame = nullptr;
}

void ADCSGameMode::UpdateInventoryValues()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (GameUtils::IsValid(PlayerCharacter))
    {
        UInventoryComponent* InventoryComp = 
            Cast<UInventoryComponent>(PlayerCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));

        if (GameUtils::IsValid(InventoryComp))
        {
            Inventory = InventoryComp->GetInventory();
        }
    }
}

void ADCSGameMode::UpdateEquipmentValues()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    if (GameUtils::IsValid(PlayerCharacter))
    {
        UEquipmentComponent* EquipComp =
            Cast<UEquipmentComponent>(PlayerCharacter->GetComponentByClass(UEquipmentComponent::StaticClass()));

        if (GameUtils::IsValid(EquipComp))
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

    if (GameUtils::IsValid(PlayerController))
    {
        TArray<UActorComponent*> StatComps;
        PlayerController->GetComponents(UExtendedStatComponent::StaticClass(), StatComps);

        for (UActorComponent* Comp : StatComps)
        {
            UExtendedStatComponent* StatComp = Cast<UExtendedStatComponent>(Comp);

            if (GameUtils::IsValid(StatComp))
            {
                CurrentStatValues.Add(StatComp->GetStatType(), StatComp->GetCurrentValue());
            }
        }
    }
}


void ADCSGameMode::LoadGame()
{
    USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameName, 0);

    SavedGame = Cast<UDCSSaveGame>(LoadedGame);

    if (GameUtils::IsValid(SavedGame))
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
    if (GameUtils::IsValid(SavedGame))
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