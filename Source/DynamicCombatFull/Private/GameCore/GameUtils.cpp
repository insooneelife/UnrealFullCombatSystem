// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUtils.h"
#include "Sound/SoundBase.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/DefaultGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"

const FLinearColor GameUtils::Red(0.53125, 0.06f, 0.0f);
const FLinearColor GameUtils::Black(0.06f, 0.06f, 0.06f);
const FLinearColor GameUtils::Gray = FLinearColor(0.5625f, 0.5625f, 0.5625f);
const FLinearColor GameUtils::Brown = FLinearColor(0.213542f, 0.0f, 0.0f);

const FItem& GameUtils::GetDefaultItemFromStoredItem(const FStoredItem& StoredItem)
{
    return GetDefaultItemFromItemClass(StoredItem.ItemClass);
}

const FItem& GameUtils::GetDefaultItemFromItemClass(TSubclassOf<UItemBase> ItemClass)
{
    UItemBase* DefaultItem = Cast<UItemBase>(ItemClass->GetDefaultObject());
    return DefaultItem->GetItem();
}








