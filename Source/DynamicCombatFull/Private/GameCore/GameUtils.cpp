// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUtils.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Sound/SoundBase.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/DefaultGameInstance.h"
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

void GameUtils::SetTimerRetriggerable(
    FTimerManager& InTimerManager,
    FTimerHandle& InTimerHandle, 
    TBaseDelegate<void> InObjectDelegate, 
    float InTime, 
    bool bInLoop)
{
    InTimerManager.ClearTimer(InTimerHandle);
    InTimerManager.SetTimer(InTimerHandle, InObjectDelegate, InTime, bInLoop);
}

void GameUtils::PrintStoredItem(const FStoredItem& InStoredItem)
{
    if (UKismetSystemLibrary::IsValidClass(InStoredItem.ItemClass))
    {
        UE_LOG(LogTemp, Error, TEXT("Print StoredItem  Id : %s  ItemClass : %s  Amount : %d"),
            *InStoredItem.Id.ToString(),
            *InStoredItem.ItemClass->GetName(),
            InStoredItem.Amount);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StoredItem is not valid!!  Id : %s"), *InStoredItem.Id.ToString());
    }
}

void GameUtils::PrintHitResult(const FHitResult& InHitResult)
{
    FVector HitLocation = InHitResult.Location;
    FVector HitNormal = InHitResult.Normal;
    FVector ImpactPoint = InHitResult.ImpactPoint;
    AActor* HitActor = InHitResult.GetActor();
    UPrimitiveComponent* HitComponent = InHitResult.GetComponent();

    UE_LOG(LogTemp, Error, TEXT("Print HitResult  HitLocation : %s   HitNormal : %s   ImpactPoint : %s   HitActor : %s   HitComponent : %s"),
        *HitLocation.ToString(),
        *HitNormal.ToString(),
        *ImpactPoint.ToString(),
        HitActor == nullptr ? TEXT("null") : *HitActor->GetName(),
        HitComponent == nullptr ? TEXT("null") : *HitComponent->GetName()
    );
}


void GameUtils::PrintHitData(const FHitData& InHitData)
{
    float Damage = InHitData.Damage;
    FVector HitFromDirection = InHitData.HitFromDirection;
    bool bCanBeParried = InHitData.bCanBeParried;
    bool bCanBeBlocked = InHitData.bCanBeBlocked;
    bool bCanReceiveImpact = InHitData.bCanReceiveImpact;
    AActor* DamageCauser = InHitData.DamageCauser;

    UE_LOG(LogTemp, Error, TEXT("Print HitData  Damage : %f  HitFromDirection : %s   bCanBeParried : %d   bCanBeBlocked : %d   bCanReceiveImpact : %d   DamageCauser : %s"),
        Damage,
        *HitFromDirection.ToString(),
        bCanBeParried,
        bCanBeBlocked,
        bCanReceiveImpact,
        DamageCauser == nullptr ? TEXT("null") : *DamageCauser->GetName()
    );
}

void GameUtils::PrintEffects(
    UObject* WorldContextObject, const FString& InDisplayName, const TArray<FEffect>& InEffects)
{
    UKismetSystemLibrary::PrintString(WorldContextObject, InDisplayName);

    FString LEffects;
    for (const FEffect& Effect : InEffects)
    {
        FString A = GetEnumValueAsString<EEffectType>("EEffectType", Effect.Type);
        FString B = UKismetStringLibrary::Conv_FloatToString(Effect.Duration);

        LEffects.Append(A);
        LEffects.Append(TEXT(":"));
        LEffects.Append(B);
        LEffects.Append(TEXT("***"));
    }

    UKismetSystemLibrary::PrintString(
        WorldContextObject, LEffects, true, true,
        FLinearColor::Red, UGameplayStatics::GetWorldDeltaSeconds(WorldContextObject));
}

bool GameUtils::IsValid(const UObjectBase* const InObject)
{
    if (InObject->IsValidLowLevel())
    {
        return true;
    }
    else
    {
        FDebug::DumpStackTraceToLog();
        //UE_LOG(LogTemp, Error, TEXT("Is not valid!!"));
        return false;
    }
}


FString GameUtils::GetDebugName(const UObject* const InObject)
{
    return InObject == nullptr ? TEXT("InValid") : *InObject->GetName();
}


void GameUtils::DrawPoint(UWorld* InWorld, FVector InLocation)
{
    DrawDebugPoint(InWorld, InLocation, 20, FColor::Red, true);
}


void GameUtils::DrawArrow(UWorld* InWorld, FVector Start, FVector End)
{
    DrawDebugDirectionalArrow(InWorld, Start, End, 12.f, FColor::Magenta, true, -1.f, 0, 5.f);
}


