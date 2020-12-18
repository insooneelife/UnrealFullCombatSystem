// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeaponItem.h"

URangeWeaponItem::URangeWeaponItem(const FObjectInitializer& ObjectInitializer)
{
    Item = FItem(
        FName(TEXT("Base Range Weapon")),
        FText::FromString(TEXT("Item description")),
        EItemType::RangeWeapon, false, true, false, nullptr);

    Modifiers =
    {
        FModifier(EStat::Damage, 25.0f),
        FModifier(EStat::AttackSpeed, 1.0f),
        FModifier(EStat::CritChance, 0.0f),
    };

    WeaponType = EWeaponType::Bow;
    bIsTwoHanded = true;
}