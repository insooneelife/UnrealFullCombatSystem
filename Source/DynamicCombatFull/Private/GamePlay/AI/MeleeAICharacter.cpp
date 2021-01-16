// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAICharacter.h"
#include "Engine/DataTable.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameCore/GameUtils.h"
#include "Components/EquipmentComponent.h"

AMeleeAICharacter::AMeleeAICharacter()
{
    static UBehaviorTree* LoadedObject =
        GameUtils::LoadAssetObject<UBehaviorTree>("/Game/DynamicCombatSystem/Blueprints/AI/Melee/MeleeAIBT");
    BTree = LoadedObject;

    static UDataTable* LoadedDataTable =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/AIMeleeMontages");
    Montages = LoadedDataTable;


    FString ObjectItemDir("/Game/DynamicCombatSystem/Blueprints/Items/ObjectItems/Instances/");
    auto SteelHelmetBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelHelmetBP"));
    auto SteelShieldBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelShieldBP"));
    auto SteelBreastplateBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelBreastplateBP"));
    auto SteelPantsBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelPantsBP"));
    auto SteelBootsBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelBootsBP"));
    auto SteelSwordBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelSwordBP"));

    Equipment->SetEquipmentSlots({
        FEquipmentSlots(EItemType::Spell, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(),
                FStoredItem(),
                FStoredItem()
            },
            0, false)
        }),

        FEquipmentSlots(EItemType::Shield, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelShieldBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::Head, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelHelmetBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::Top, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelBreastplateBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::Legs, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelPantsBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::Hands, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Feet, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelBootsBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::Arrows, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Tool, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(),
                FStoredItem()
            },
            0, false)
        }),
        FEquipmentSlots(EItemType::MeleeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelSwordBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::RangeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        })
        });
}