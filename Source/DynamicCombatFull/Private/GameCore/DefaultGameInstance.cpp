// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameInstance.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Math/NumericLimits.h"
#include "GameFramework/InputSettings.h"
#include "Components/EquipmentComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/BehaviorComponent.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"

#include "GamePlay/Items/ObjectItems/ArrowItem.h"

const FName UDefaultGameInstance::Inventory = FName("Inventory");
const FName UDefaultGameInstance::UIBack = FName("UIBack");
const FName UDefaultGameInstance::UITakeAll = FName("UITakeAll");
const FName UDefaultGameInstance::Equipment = FName("Equipment");
const FName UDefaultGameInstance::Interact = FName("Interact");
const FName UDefaultGameInstance::TakeAll = FName("TakeAll");
const FName UDefaultGameInstance::Back = FName("Back");

UDefaultGameInstance::UDefaultGameInstance(const FObjectInitializer& ObjectInitializer)
{


    /*
    TSubclassOf<UItemBase> ArrowItemClass = GameUtils::LoadAssetClass<UItemBase>(
        "/Game/DynamicCombatSystem/Blueprints/Test/TestTestItem");

    UItemBase* ArrowItem = Cast<UItemBase>(ArrowItemClass->GetDefaultObject());

    if (ArrowItem->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *ArrowItem->GetFName().ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Not Valid Arrow Item!!!!!!"));
    }
    */

    FString FindObjName = TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_HitHands");
    ConstructorHelpers::FObjectFinder<USoundBase> DefaultHitSoundObjFinder(*FindObjName);
    if (DefaultHitSoundObjFinder.Object == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find : %s"), *FindObjName);
        return;
    }

    DefaultHitSound = DefaultHitSoundObjFinder.Object;


    FindObjName = TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_HitSword");
    ConstructorHelpers::FObjectFinder<USoundBase> SwordHitSoundObjFinder(*FindObjName);
    if (SwordHitSoundObjFinder.Object == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find : %s"), *FindObjName);
        return;
    }

    SwordHitSound = SwordHitSoundObjFinder.Object;

    AxeHitSound = SwordHitSoundObjFinder.Object;

    FindObjName = TEXT("/Game/DynamicCombatSystem/SFX/CUE/Cue_BlockShieldMetal");
    ConstructorHelpers::FObjectFinder<USoundBase> BlockShieldSoundObjFinder(*FindObjName);
    if (BlockShieldSoundObjFinder.Object == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot find : %s"), *FindObjName);
        return;
    }

    BlockShieldSound = BlockShieldSoundObjFinder.Object;
}


void UDefaultGameInstance::PlayHitSound(AActor* Applier, AActor* Receiver, FVector Location)
{
    UEquipmentComponent* ApplierActorEquip =
        Cast<UEquipmentComponent>(Applier->GetComponentByClass(UEquipmentComponent::StaticClass()));

    UEquipmentComponent* ReceiverActorEquip =
        Cast<UEquipmentComponent>(Receiver->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (ApplierActorEquip->IsValidLowLevel() && ReceiverActorEquip->IsValidLowLevel())
    {
        USoundBase* Sound = DefaultHitSound;

        if (ApplierActorEquip->GetWeaponType() == EWeaponType::Sword)
        {
            Sound = SwordHitSound;
        }
        else if (ApplierActorEquip->GetWeaponType() == EWeaponType::Axe)
        {
            Sound = AxeHitSound;
        }

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
    }
}

void UDefaultGameInstance::PlayParrySound(AActor* Applier, AActor* Receiver, FVector Location)
{
    UEquipmentComponent* ApplierActorEquip =
        Cast<UEquipmentComponent>(Applier->GetComponentByClass(UEquipmentComponent::StaticClass()));

    UEquipmentComponent* ReceiverActorEquip =
        Cast<UEquipmentComponent>(Receiver->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (ApplierActorEquip->IsValidLowLevel() && ReceiverActorEquip->IsValidLowLevel())
    {
        USoundBase* Sound = DefaultHitSound;

        if (ApplierActorEquip->IsShieldEquipped())
        {
            Sound = BlockShieldSound;
        }
        else
        {
            if (ReceiverActorEquip->IsWeaponEquipped())
            {
                if (ReceiverActorEquip->GetWeaponType() == EWeaponType::Sword)
                {
                    Sound = BlockShieldSound;
                }
                else if (ReceiverActorEquip->GetWeaponType() == EWeaponType::Axe)
                {
                    Sound = BlockShieldSound;
                }
                else if (ReceiverActorEquip->GetWeaponType() == EWeaponType::Bow)
                {
                    Sound = BlockShieldSound;
                }
            }
        }

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
    }
}

void UDefaultGameInstance::PlayBlockSound(AActor* Applier, AActor* Receiver, FVector Location)
{
    UEquipmentComponent* ApplierActorEquip =
        Cast<UEquipmentComponent>(Applier->GetComponentByClass(UEquipmentComponent::StaticClass()));

    UEquipmentComponent* ReceiverActorEquip =
        Cast<UEquipmentComponent>(Receiver->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (ApplierActorEquip->IsValidLowLevel() && ReceiverActorEquip->IsValidLowLevel())
    {
        USoundBase* Sound = DefaultHitSound;

        if (ApplierActorEquip->IsShieldEquipped())
        {
            if (ReceiverActorEquip->GetWeaponType() == EWeaponType::Spell)
            {

            }
            else
            {
                Sound = BlockShieldSound;
            }
        }
        else
        {
            if (ApplierActorEquip->IsWeaponEquipped())
            {
                if (ApplierActorEquip->GetWeaponType() == EWeaponType::Sword)
                {
                    Sound = BlockShieldSound;
                }
                else if (ApplierActorEquip->GetWeaponType() == EWeaponType::Axe)
                {
                    Sound = BlockShieldSound;
                }
                else if (ApplierActorEquip->GetWeaponType() == EWeaponType::Bow)
                {
                    Sound = BlockShieldSound;
                }
                else if (ApplierActorEquip->GetWeaponType() == EWeaponType::Spell)
                {
                    Sound = BlockShieldSound;
                }
            }
            else
            {
                Sound = DefaultHitSound;
            }
        }

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
    }
}

EMontageAction UDefaultGameInstance::ConvertMeleeAttackTypeToAction(EMeleeAttackType Type)
{
    if (Type == EMeleeAttackType::None)
    {
        return EMontageAction::None;
    }
    else if (Type == EMeleeAttackType::Light)
    {
        return EMontageAction::LightAttack;
    }
    else if (Type == EMeleeAttackType::Heavy)
    {
        return EMontageAction::HeavyAttack;
    }
    else if (Type == EMeleeAttackType::Special)
    {
        return EMontageAction::SpecialAttack;
    }
    else if (Type == EMeleeAttackType::Thrust)
    {
        return EMontageAction::ThrustAttack;
    }
    else if (Type == EMeleeAttackType::Falling)
    {
        return EMontageAction::FallingAttack;
    }

    return EMontageAction::None;
}

UExtendedStatComponent* UDefaultGameInstance::GetExtendedStatComponent(AActor* Actor, EStat Type)
{
    TArray<UActorComponent*> Comps;
    Actor->GetComponents(UExtendedStatComponent::StaticClass(), Comps);

    for (UActorComponent* Comp : Comps)
    {
        UExtendedStatComponent* StatComp = Cast<UExtendedStatComponent>(Comp);
        if (StatComp != nullptr)
        {
            if (StatComp->GetStatType() == Type)
            {
                return StatComp;
            }
        }
    }

    return nullptr;
}

TArray<AActor*> UDefaultGameInstance::SelectEnemyActors(AActor* Target, const TArray<AActor*>& Actors)
{
    TArray<AActor*> EnemyActors;

    if (Target->IsValidLowLevel())
    {
        UBehaviorComponent* Comp = Cast<UBehaviorComponent>(Target->GetComponentByClass(UBehaviorComponent::StaticClass()));

        if (Comp->IsValidLowLevel())
        {
            for (AActor* Actor : Actors)
            {
                if (Comp->IsEnemy(Actor))
                {
                    EnemyActors.Add(Actor);
                }
            }
        }
    }

    return EnemyActors;
}

AActor* UDefaultGameInstance::GetClosestActor(AActor* Target, const TArray<AActor*>& Actors)
{
    AActor* ClosestActor = nullptr;
    float MinDistance = TNumericLimits<float>::Max();

    if (Target->IsValidLowLevel())
    {
        for (AActor* Actor : Actors)
        {
            float Distance = Target->GetSquaredDistanceTo(Actor);

            if (MinDistance > Distance)
            {
                MinDistance = Distance;
                ClosestActor = Actor;
            }
        }
    }

    return ClosestActor;
}

FKey UDefaultGameInstance::GetFirstActionMappingKey(FName ActionName)
{
    UInputSettings* InputSettings = UInputSettings::GetInputSettings();
    if (InputSettings->IsValidLowLevel())
    {
        TArray<FInputActionKeyMapping> OutMappings;
        InputSettings->GetActionMappingByName(ActionName, OutMappings);

        return OutMappings[0].Key;
    }
    else
    {
        return FKey();
    }
}

EDirection UDefaultGameInstance::GetHitDirection(FVector HitFromDirection, AActor* AttackedActor)
{
    if (AttackedActor->IsValidLowLevel())
    {
        FVector Forward = AttackedActor->GetActorForwardVector();
        Forward.Z = 0.0f;
        Forward.Normalize();

        FVector HitDir = HitFromDirection;
        HitDir.Z = 0.0f;
        HitDir.Normalize();

        float Dot = FVector::DotProduct(Forward, HitDir);

        if (Dot >= 0.25f)
        {
            return EDirection::Back;
        }
        else if (Dot <= -0.25f)
        {
            return EDirection::Front;
        }
        else
        {
            FVector Right = AttackedActor->GetActorRightVector();
            Right.Z = 0.0f;
            Right.Normalize();

            if (FVector::DotProduct(Right, HitDir))
            {
                return EDirection::Right;
            }
            else
            {
                return EDirection::Left;
            }
        }
    }
    else
    {
        return EDirection::Front;
    }
}

float UDefaultGameInstance::ScaleMeleeAttackStaminaCostByType(float Cost, EMeleeAttackType MeleeAttackType)
{
    float Ret = 1.0f;
    if (MeleeAttackType == EMeleeAttackType::None)
    {
        Ret = 1.0f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Light)
    {
        Ret = 1.0f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Heavy)
    {
        Ret = 1.75f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Special)
    {
        Ret = 1.75f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Thrust)
    {
        Ret = 1.75f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Falling)
    {
        Ret = 0.75f;
    }

    return Ret * Cost;
}

float UDefaultGameInstance::ScaleMeleeDamageByType(float Damage, EMeleeAttackType MeleeAttackType)
{
    float Ret = 1.0f;
    if (MeleeAttackType == EMeleeAttackType::None)
    {
        Ret = 1.0f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Light)
    {
        Ret = 1.0f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Heavy)
    {
        Ret = 1.5f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Special)
    {
        Ret = 1.5f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Thrust)
    {
        Ret = 1.5f;
    }
    else if (MeleeAttackType == EMeleeAttackType::Falling)
    {
        Ret = 1.0f;
    }
    return Ret;
}