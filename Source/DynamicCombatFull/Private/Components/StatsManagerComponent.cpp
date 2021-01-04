// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsManagerComponent.h"
#include "Components/EquipmentComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/ItemHasModifiers.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"
#include "Components/ExtendedStatComponent.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"

// Sets default values for this component's properties
UStatsManagerComponent::UStatsManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
	
    Stats = {
        FStat(EStat::Health, 100.0f, 0.0f),
        FStat(EStat::Stamina, 100.0f, 0.0f),
        FStat(EStat::Mana, 100.0f, 0.0f),
        FStat(EStat::Damage, 10.0f, 0.0f),
        FStat(EStat::Armor, 0.0f, 0.0f),
        FStat(EStat::CritChance, 0.0f, 0.0f),
        FStat(EStat::CritMultiplier, 0.0f, 0.0f),
        FStat(EStat::AttackSpeed, 1.0f, 0.0f),
        FStat(EStat::Block, 50.0f, 0.0f),
        FStat(EStat::MeleeAttackStaminaCost, 15.0f, 0.0f),
        FStat(EStat::MagicDamage, 10.0f, 0.0f),
        FStat(EStat::CastingSpeed, 1.0f, 0.0f)
    };
    
}

void UStatsManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GameUtils::IsValid(EquipmentComponent))
    {
        EquipmentComponent->OnActiveItemChanged.RemoveDynamic(this, &UStatsManagerComponent::OnActiveItemChanged);
        EquipmentComponent->OnSlotHiddenChanged.RemoveDynamic(this, &UStatsManagerComponent::OnSlotHiddenChanged);
        EquipmentComponent->OnMainHandTypeChanged.RemoveDynamic(this, &UStatsManagerComponent::OnMainHandTypeSwitched);
    }

    Super::EndPlay(EndPlayReason);
}

void UStatsManagerComponent::Init()
{
    InitialBlockValue = GetStatValue(EStat::Block, false);
    EquipmentComponent = Cast<UEquipmentComponent>(GetOwner()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (GameUtils::IsValid(EquipmentComponent))
    {
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UStatsManagerComponent::OnActiveItemChanged);
        EquipmentComponent->OnSlotHiddenChanged.AddDynamic(this, &UStatsManagerComponent::OnSlotHiddenChanged);
        EquipmentComponent->OnMainHandTypeChanged.AddDynamic(this, &UStatsManagerComponent::OnMainHandTypeSwitched);
    }
}

void UStatsManagerComponent::TakeDamage(float InDamage, bool bInIgnoreStamina)
{
    GetWorld()->GetTimerManager().ClearTimer(ResetRecentlyReceivedDamageTimerHandle);

    RecentlyReceivedDamage += InDamage;
    RecentlyReceivedHitsCount++;

    GetWorld()->GetTimerManager().SetTimer(
        ResetRecentlyReceivedDamageTimerHandle, this, &UStatsManagerComponent::ResetRecentlyReceivedDamage, 4.0f, false);

    UExtendedStatComponent* HealthStatComp = UDefaultGameInstance::GetExtendedStatComponent(GetOwner(), EStat::Health);
    UExtendedStatComponent* StaminaStatComp = UDefaultGameInstance::GetExtendedStatComponent(GetOwner(), EStat::Stamina);

    float ArmorVal = InDamage * (GetStatValue(EStat::Armor, true) / 100.0f);
    float ReducedDamage = FMath::Clamp(InDamage - ArmorVal, 0.0f, InDamage - ArmorVal);

    if (GameUtils::IsValid(HealthStatComp))
    {
        if (bInIgnoreStamina || !GameUtils::IsValid(StaminaStatComp))
        {
            HealthStatComp->ModifyStat(ReducedDamage * -1.0f, true);

            // ???
            RecentlyReceivedSuccessfulDamage += InDamage;
            RecentlyReceivedSuccessfulHitsCount++;
        }
        else
        {
            float BlockedDamage = ReducedDamage * (GetStatValue(EStat::Block, true) / 100.0f);

            float StaminaValue = StaminaStatComp->GetCurrentValue();
            if (StaminaValue >= BlockedDamage)
            {
                StaminaStatComp->ModifyStat(BlockedDamage * (-1.25f), true);
                HealthStatComp->ModifyStat((ReducedDamage - BlockedDamage) * -1.0f, true);
            }
            else
            {
                StaminaStatComp->SetCurrentValue(0.0f, true);
                BlockedDamage = StaminaValue;
                HealthStatComp->ModifyStat((ReducedDamage - BlockedDamage) * -1.0f, true);

                RecentlyReceivedSuccessfulDamage += InDamage;
                RecentlyReceivedSuccessfulHitsCount++;
            }

        }
    }
}

void UStatsManagerComponent::AddModifier(EStat InType, float InValue)
{
    if (InValue != 0.0f)
    {
        int Index = GetStatIndex(InType);

        if (Index >= 0)
        {
            Stats[Index].ModifiersValue += InValue;
            OnModifierAdded.Broadcast(InType, InValue);
        }
    }
}

void UStatsManagerComponent::RemoveModifier(EStat InType, float InValue)
{
    if (InValue != 0.0f)
    {
        int Index = GetStatIndex(InType);

        if (Index >= 0)
        {
            Stats[Index].ModifiersValue -= InValue;

            OnModifierRemoved.Broadcast(InType, InValue);
        }
    }
}

float UStatsManagerComponent::GetStatValue(EStat InType, bool bInIncludeModifiers) const
{
    int Index = GetStatIndex(InType);

    if (Index >= 0)
    {
        float BaseValue = Stats[Index].BaseValue;
        float ModifierValue = Stats[Index].ModifiersValue;

        if (bInIncludeModifiers)
        {
            return BaseValue + ModifierValue;
        }
        else
        {
            return BaseValue;
        }
    }

    return 0.0f;
}

float UStatsManagerComponent::GetDamage() const
{
    float Damage = GetStatValue(EStat::Damage, true);

    if (UKismetMathLibrary::RandomFloatInRange(1.0f, 100.0f) <= GetStatValue(EStat::CritChance, true))
    {
        float CritMultiplier = GetStatValue(EStat::CritMultiplier, true);
        Damage *= CritMultiplier;
    }

    return UKismetMathLibrary::FTrunc(Damage);
}

void UStatsManagerComponent::OnActiveItemChanged(
    FStoredItem InOldItem, FStoredItem InNewItem, EItemType InSlotType, int InSlotIndex, int InActiveIndex)
{
    UpdateBlockBaseValue();
    
    if (!EquipmentComponent->IsSlotHidden(InSlotType, InSlotIndex))
    {
        IncludeItemModifiers(InOldItem.ItemClass);
        IncludeItemModifiers(InNewItem.ItemClass);        
    }
}

void UStatsManagerComponent::OnSlotHiddenChanged(
    EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden)
{
    if (bInIsHidden)
    {
        ExcludeItemModifiers(InActiveItem.ItemClass);
    }
    else
    {
        IncludeItemModifiers(InActiveItem.ItemClass);
    }
}

void UStatsManagerComponent::OnMainHandTypeSwitched(EItemType InSlotType)
{
    UpdateBlockBaseValue();
}

void UStatsManagerComponent::ExcludeItemModifiers(TSubclassOf<UItemBase> InItem)
{
    if (UKismetSystemLibrary::IsValidClass(InItem))
    {
        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), InItem);

        IItemHasModifiers* ItemHasModifers = Cast<IItemHasModifiers>(ItemBase);
        if (ItemHasModifers != nullptr)
        {
            TArray<FModifier> Modifiers = ItemHasModifers->GetModifiers();

            for (FModifier Modifier : Modifiers)
            {
                RemoveModifier(Modifier.Type, Modifier.Value);
            }
        }
    }
}

void UStatsManagerComponent::IncludeItemModifiers(TSubclassOf<UItemBase> InItem)
{
    if (UKismetSystemLibrary::IsValidClass(InItem))
    {
        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), InItem);

        IItemHasModifiers* ItemHasModifers = Cast<IItemHasModifiers>(ItemBase);
        if (ItemHasModifers != nullptr)
        {
            TArray<FModifier> Modifiers = ItemHasModifers->GetModifiers();

            for (FModifier Modifier : Modifiers)
            {
                AddModifier(Modifier.Type, Modifier.Value);
            }
        }
    }
}

int UStatsManagerComponent::GetStatIndex(EStat InType) const
{
    for (int i = 0; i < Stats.Num(); ++i)
    {
        if (Stats[i].Type == InType)
        {
            return i;
        }
    }

    return -1;
}

void UStatsManagerComponent::ChangeStatBaseValue(EStat InType, float InNewValue)
{
    int Index = GetStatIndex(InType);
    Stats[Index].BaseValue = InNewValue;

    OnBaseValueChanged.Broadcast(InType, InNewValue);
}

void UStatsManagerComponent::ResetRecentlyReceivedDamage()
{
    RecentlyReceivedDamage = 0.0f;
    RecentlyReceivedHitsCount = 0.0f;
    RecentlyReceivedSuccessfulDamage = 0.0f;
    RecentlyReceivedSuccessfulHitsCount = 0.0f;
}

void UStatsManagerComponent::UpdateBlockBaseValue()
{
    float Value = 0.0f;
    if (EquipmentComponent->GetBlockValue(Value))
    {
    }
    else
    {
        Value = InitialBlockValue;
    }

    ChangeStatBaseValue(EStat::Block, Value);
}
