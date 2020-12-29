// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"
#include "GameCore/GameUtils.h"

URangeWeaponItem::URangeWeaponItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_RangeWeapon");

    Item = FItem(
        FName(TEXT("Base Range Weapon")),
        FText::FromString(TEXT("Item description")),
        EItemType::RangeWeapon, false, true, false, LoadTexture);

    Modifiers =
    {
        FModifier(EStat::Damage, 25.0f),
        FModifier(EStat::AttackSpeed, 1.0f),
        FModifier(EStat::CritChance, 0.0f),
    };

    WeaponType = EWeaponType::Bow;
    bIsTwoHanded = true;
}