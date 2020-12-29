// Fill out your copyright notice in the Description page of Project Settings.
#include "ShieldItem.h"
#include "GameCore/GameUtils.h"

UShieldItem::UShieldItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_Shield");

    Item = FItem(
        FName(TEXT("Base Shield")),
        FText::FromString(TEXT("Item description")),
        EItemType::Shield, false, true, false, LoadTexture);

    BlockValue = 100.0f;
}

