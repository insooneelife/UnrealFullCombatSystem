// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldItem.h"
UShieldItem::UShieldItem(const FObjectInitializer& ObjectInitializer)
{
    Item = FItem(
        FName(TEXT("Base Shield")),
        FText::FromString(TEXT("Item description")),
        EItemType::Shield, false, true, false, nullptr);

    BlockValue = 100.0f;
}

