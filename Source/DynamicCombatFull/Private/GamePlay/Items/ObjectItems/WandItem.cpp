// Fill out your copyright notice in the Description page of Project Settings.
#include "WandItem.h"
#include "GameCore/GameUtils.h"

UWandItem::UWandItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_Wand");

    Item = FItem(
        FName(TEXT("Base Wand")),
        FText::FromString(TEXT("Item description")),
        EItemType::Shield, false, true, false, LoadTexture);


    Modifiers =
    {
        FModifier(EStat::MagicDamage, 5.0f),
        FModifier(EStat::CastingSpeed, 0.1f),
    };
}