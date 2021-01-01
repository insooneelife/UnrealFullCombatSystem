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

void GameUtils::PrintStoredItem(const FStoredItem& StoredItem)
{
    if (StoredItem.ItemClass->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("Print StoredItem  Id : %s  ItemClass : %s  Amount : %d"),
            *StoredItem.Id.ToString(),
            *StoredItem.ItemClass->GetName(),
            StoredItem.Amount);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StoredItem is not valid!!  Id : %s"), *StoredItem.Id.ToString());
    }
}

void GameUtils::PrintHitResult(const FHitResult& InHit)
{
    FVector HitLocation = InHit.Location;
    FVector HitNormal = InHit.Normal;
    FVector ImpactPoint = InHit.ImpactPoint;
    AActor* HitActor = InHit.GetActor();
    UPrimitiveComponent* HitComponent = InHit.GetComponent();

    UE_LOG(LogTemp, Error, TEXT("Print HitLocation : %s   HitNormal : %s   ImpactPoint : %s   HitActor : %s   HitComponent : %s"),
        *HitLocation.ToString(),
        *HitNormal.ToString(),
        *ImpactPoint.ToString(),
        HitActor == nullptr ? TEXT("") : *HitActor->GetName(),
        HitComponent == nullptr ? TEXT("") : *HitComponent->GetName()
    );
}




