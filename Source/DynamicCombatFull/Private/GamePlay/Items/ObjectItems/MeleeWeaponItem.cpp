// Fill out your copyright notice in the Description page of Project Settings.
#include "MeleeWeaponItem.h"
#include "GameCore/GameUtils.h"

UMeleeWeaponItem::UMeleeWeaponItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_MeleeWeapon");

    Item = FItem(
        FName(TEXT("Base Melee Weapon")),
        FText::FromString(TEXT("Item description")),
        EItemType::MeleeWeapon, false, true, false, LoadTexture);

    WeaponType = EWeaponType::Sword;

    Modifiers =
    {
        FModifier(EStat::Damage, 10.0f),
        FModifier(EStat::AttackSpeed, 0.0f),
        FModifier(EStat::MeleeAttackStaminaCost, 0.0f),
        FModifier(EStat::CritChance, 0.0f),
    };

    BlockValue = 50.0f;
    bIsTwoHanded = false;
}

