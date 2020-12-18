// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowItem.h"

UArrowItem::UArrowItem(const FObjectInitializer& ObjectInitializer)
{
    Item = FItem(
        FName(TEXT("Base Arrow")),
        FText::FromString(TEXT("Item description")),
        EItemType::Arrows, true, true, false, nullptr);

    Modifiers =
    {
        FModifier(EStat::Damage, 10.0f)
    };
}