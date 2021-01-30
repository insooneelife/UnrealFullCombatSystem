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
}