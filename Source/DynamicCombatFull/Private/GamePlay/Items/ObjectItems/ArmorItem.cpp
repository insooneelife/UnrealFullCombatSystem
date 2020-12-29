// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItem.h"
#include "GameCore/GameUtils.h"

UArmorItem::UArmorItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_Top");

    Item = FItem(
        FName(TEXT("None")),
        FText::FromString(TEXT("Item description")),
        EItemType::None, false, true, false, LoadTexture);

    Modifiers =
    {
        FModifier(EStat::Armor, 0.0f),
        FModifier(EStat::Health, 0.0f),
        FModifier(EStat::Stamina, 0.0f),
        FModifier(EStat::Mana, 0.0f)
    };

}
