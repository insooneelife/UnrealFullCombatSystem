// Fill out your copyright notice in the Description page of Project Settings.


#include "MageAICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Sound/SoundBase.h"
#include "Engine/DataTable.h"
#include "Components/EquipmentComponent.h"
#include "Components/AbilityComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "GamePlay/AI/BaseAIController.h"
#include "GameCore/GameUtils.h"

AMageAICharacter::AMageAICharacter()
    :bIsTargetable(true)
{
    static UBehaviorTree* LoadedObject =
        GameUtils::LoadAssetObject<UBehaviorTree>("/Game/DynamicCombatSystem/Blueprints/AI/Mage/MageAIBT");
    BTree = LoadedObject;

    static UDataTable* LoadedDataTable =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/AIMageMontages");
    AIMageMontages = LoadedDataTable;

    ExtendedMana = CreateDefaultSubobject<UExtendedStatComponent>("ExtendedMana");
    AbilityComponent = CreateDefaultSubobject<UAbilityComponent>("AbilityComponent");

    AbilityComponent->SetUpdateEquipmentAbility(false);

    ExtendedMana->SetStatType(EStat::Mana);
    ExtendedMana->SetDoesRegenerates(true);
    ExtendedMana->SetRegenValue(2.0f);
    ExtendedMana->SetReenableRegenTime(1.5f);

    FString ObjectItemDir("/Game/DynamicCombatSystem/Blueprints/Items/ObjectItems/Instances/");
    auto MagicWandBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("MagicWandBP"));
    auto FireballBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("FireballBP"));

    //auto InfernoBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("InfernoBP"));
    //auto InstantHealBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("InstantHealBP"));
    //auto TeleportBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("TeleportBP"));

    Equipment->SetEquipmentSlots({
        FEquipmentSlots(EItemType::Spell, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(FGuid::NewGuid(), FireballBPClass, 1),
                FStoredItem(),
                FStoredItem()
            },
            0, false)
        }),

        FEquipmentSlots(EItemType::Shield, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), MagicWandBPClass, 1)}, 0, false)
        }),

        FEquipmentSlots(EItemType::Head, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Top, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Legs, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Hands, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Feet, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Arrows, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Tool, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(),
                FStoredItem()
            },
            0, false)
        }),
        FEquipmentSlots(EItemType::MeleeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem()
            },
            0, false)
        }),
        FEquipmentSlots(EItemType::RangeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem()
            },
            0, false)
        })
        });
}

void AMageAICharacter::BeginPlay()
{
    Super::BeginPlay();

    Equipment->SetMainHandType(EItemType::Spell);

    AbilityComponent->OnAbilityStarted.AddDynamic(this, &AMageAICharacter::OnAbilityStarted);
    AbilityComponent->OnAbilityEnded.AddDynamic(this, &AMageAICharacter::OnAbilityEnded);

    AbilityComponent->OnManaConsumed.AddDynamic(this, &AMageAICharacter::OnManaConsumed);
    StateManager->OnStateChanged.AddDynamic(this, &AMageAICharacter::OnStateChanged);
}

void AMageAICharacter::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    AbilityComponent->OnAbilityStarted.RemoveDynamic(this, &AMageAICharacter::OnAbilityStarted);
    AbilityComponent->OnAbilityEnded.RemoveDynamic(this, &AMageAICharacter::OnAbilityEnded);

    AbilityComponent->OnManaConsumed.RemoveDynamic(this, &AMageAICharacter::OnManaConsumed);
    StateManager->OnStateChanged.RemoveDynamic(this, &AMageAICharacter::OnStateChanged);

    Super::EndPlay(EndPlayResult);
}

void AMageAICharacter::MakeUntargetable(float InDuration)
{
    bIsTargetable = false;

    GameUtils::SetTimerRetriggerable(
        GetWorld()->GetTimerManager(),
        RetriggerableDelayTimerHandle,
        FTimerDelegate::CreateUObject(this, &AMageAICharacter::Delayed_Set),
        InDuration,
        false);
}

float AMageAICharacter::GetCurrentMana() const
{
    return ExtendedMana->GetCurrentValue();
}

void AMageAICharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
    EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;

    if (NewMovementMode != EMovementMode::MOVE_Walking)
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
    }
}

UDataTable* AMageAICharacter::GetMontages(EMontageAction InAction) const
{
    return AIMageMontages;
}

FRotator AMageAICharacter::GetDesiredRotation() const
{
    if (AbilityComponent->IsCasting())
    {
        FTransform EffectTransform = AbilityComponent->GetEffectTransform();
        float Yaw = EffectTransform.Rotator().Yaw;
        FRotator ActorRot = GetActorRotation();

        FRotator Rot;
        Rot.Roll = ActorRot.Roll;
        Rot.Pitch = ActorRot.Pitch;
        Rot.Yaw = Yaw;
        return Rot;
    }
    else
    {
        return AAICharacter::GetDesiredRotation();
    }
}

bool AMageAICharacter::IsTargetable() const
{
    return AAICharacter::IsTargetable() && bIsTargetable;
}

bool AMageAICharacter::CanCastAbility() const
{
    return StateManager->GetState() == EState::Idle && 
        ExtendedMana->GetCurrentValue() > AbilityComponent->GetManaCost();
}

float AMageAICharacter::GetMagicDamage() const
{
    return StatsManager->GetStatValue(EStat::MagicDamage, true);
}

float AMageAICharacter::GetCastingSpeed() const
{
    return StatsManager->GetStatValue(EStat::CastingSpeed, true);
}

void AMageAICharacter::OnAbilityStarted()
{
    StateManager->SetState(EState::Attacking);
}

void AMageAICharacter::OnAbilityEnded(EAbilityEndResult InEndResult)
{
    if (IsStateEqual(EState::Attacking))
    {
        StateManager->ResetState(0.0f);
    }
}

void AMageAICharacter::OnManaConsumed(float InAmount)
{
    ExtendedMana->ModifyStat(InAmount * -1.0f, true);
}

void AMageAICharacter::OnStateChanged(EState InPrevState, EState InNewState)
{
    if (InNewState != EState::Idle && InNewState != EState::Attacking)
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
    }
}

void AMageAICharacter::Delayed_Set()
{
    bIsTargetable = true;
}

bool AMageAICharacter::IsStateEqual(EState InState)
{
    if (StateManager->GetState() == InState)
    {
        return true;
    }

    return false;
}
