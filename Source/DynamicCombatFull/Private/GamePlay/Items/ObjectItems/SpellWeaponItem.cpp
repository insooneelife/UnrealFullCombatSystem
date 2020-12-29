// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellWeaponItem.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"

USpellWeaponItem::USpellWeaponItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_Spell");

    Item = FItem(
        FName(TEXT("None")),
        FText::FromString(TEXT("Item description")),
        EItemType::Spell, false, false, false, LoadTexture);

    WeaponType = EWeaponType::Spell;
}
