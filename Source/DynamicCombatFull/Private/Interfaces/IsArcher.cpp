// Fill out your copyright notice in the Description page of Project Settings.


#include "IsArcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Items/ObjectItems/ArrowItem.h"
#include "GamePlay/Projectiles/ArrowProjectileBase.h"
#include "Components/EquipmentComponent.h"
#include "Components/StatsManagerComponent.h"

void IIsArcher::SpawnArrowProjectile(const FStoredItem& Item)
{
    ACharacter* ThisCharacter = GetThisCharacter();
    UArrowItem* ArrowItem = Item.ItemClass->GetDefaultObject<UArrowItem>();

    if (!GameUtils::IsValid(ArrowItem))
    {
        UE_LOG(LogTemp, Error, TEXT("ArrowItem is not valid!  Item Id : %s"), *Item.Id.ToString());
        GameUtils::PrintStoredItem(Item);
    }

    TSubclassOf<AActor> ProjectileClass = ArrowItem->GetProjectile();

    if (!UKismetSystemLibrary::IsValidClass(ProjectileClass))
    {
        UE_LOG(LogTemp, Error, TEXT("ProjectileClass is not valid!  Item Id : %s"), *Item.Id.ToString());
        GameUtils::PrintStoredItem(Item);
    }

    FTransform SpawnTransform = GetSpawnArrowTransform();
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.Owner = ThisCharacter;
    SpawnInfo.Instigator = ThisCharacter;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AArrowProjectileBase* ProjectileBase =
        ThisCharacter->GetWorld()->SpawnActor<AArrowProjectileBase>(ProjectileClass, SpawnTransform, SpawnInfo);

    float AimAlpha = GetAimAlpha();
    float ArrowInitialSpeed = GetArrowInitialSpeed();

    if (GameUtils::IsValid(ProjectileBase))
    {
        float Damage = GetRangeDamage() * AimAlpha;
        ProjectileBase->Init(Damage, ArrowInitialSpeed * AimAlpha);
    }
}


FName IIsArcher::GetBowStringSocketName() const
{
    return FName("bow_string");
}

bool IIsArcher::CanBowAttack() const
{
    UEquipmentComponent* Equipment = GetEquipment();

    return
        Equipment->IsInCombat() &&
        Equipment->AreArrowsEquipped() &&
        Equipment->GetCombatType() == ECombatType::Range;
}


float IIsArcher::GetRangeDamage() const
{
    UStatsManagerComponent* StatsManager = GetStatsManager();
    return StatsManager->GetDamage();
}