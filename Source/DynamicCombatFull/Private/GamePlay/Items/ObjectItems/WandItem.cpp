// Fill out your copyright notice in the Description page of Project Settings.


#include "WandItem.h"

UWandItem::UWandItem(const FObjectInitializer& ObjectInitializer)
{
    Item = FItem(
        FName(TEXT("Base Wand")),
        FText::FromString(TEXT("Item description")),
        EItemType::Shield, false, true, false, nullptr);


    Modifiers =
    {
        FModifier(EStat::MagicDamage, 5.0f),
        FModifier(EStat::CastingSpeed, 0.1f),
    };
}