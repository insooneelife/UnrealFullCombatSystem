// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellWeaponItem.h"


USpellWeaponItem::USpellWeaponItem(const FObjectInitializer& ObjectInitializer)
{
    Item = FItem(
        FName(TEXT("None")),
        FText::FromString(TEXT("Item description")),
        EItemType::None, false, false, false, nullptr);

    WeaponType = EWeaponType::Spell;

}
