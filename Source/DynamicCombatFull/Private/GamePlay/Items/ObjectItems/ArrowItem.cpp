// Fill out your copyright notice in the Description page of Project Settings.
#include "ArrowItem.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Projectiles/ArrowProjectileBase.h"

UArrowItem::UArrowItem(const FObjectInitializer& ObjectInitializer)
{
    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_Quiver");

    Item = FItem(
        FName(TEXT("Base Arrow")),
        FText::FromString(TEXT("Item description")),
        EItemType::Arrows, true, true, false, LoadTexture);

    Modifiers =
    {
        FModifier(EStat::Damage, 10.0f)
    };
}

void UArrowItem::BeginDestroy()
{
    Super::BeginDestroy();

    ArrowMesh = nullptr;
}