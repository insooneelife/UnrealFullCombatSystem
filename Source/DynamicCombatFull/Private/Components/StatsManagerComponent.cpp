// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsManagerComponent.h"
#include "Components/EquipmentComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/ItemHasModifiers.h"
#include "GameCore/DefaultGameInstance.h"
#include "Components/ExtendedStatComponent.h"

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


// Called when the game starts
void UStatsManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UStatsManagerComponent::OnActiveItemChanged(
    FStoredItem OldItem, FStoredItem NewItem, EItemType SlotType, int SlotIndex, int ActiveIndex)
{
    UpdateBlockBaseValue();
    
    if (!EquipmentComponent->IsSlotHidden(SlotType, SlotIndex))
    {
        IncludeItemModifiers(OldItem.ItemClass);
        IncludeItemModifiers(NewItem.ItemClass);        
    }
}

void UStatsManagerComponent::OnSlotHiddenChanged(EItemType SlotType, int SlotIndex, FStoredItem ActiveItem, bool bIsHidden)
{
    if (bIsHidden)
    {
        ExcludeItemModifiers(ActiveItem.ItemClass);
    }
    else
    {
        IncludeItemModifiers(ActiveItem.ItemClass);
    }
}

void UStatsManagerComponent::OnMainHandTypeSwitched(EItemType SlotType)
{
    UpdateBlockBaseValue();
}

void UStatsManagerComponent::Initialize()
{
    InitialBlockValue = GetStatValue(EStat::Block, false);
    EquipmentComponent = Cast<UEquipmentComponent>(GetOwner()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (EquipmentComponent->IsValidLowLevel())
    {
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UStatsManagerComponent::OnActiveItemChanged);
        EquipmentComponent->OnSlotHiddenChanged.AddDynamic(this, &UStatsManagerComponent::OnSlotHiddenChanged);
        EquipmentComponent->OnMainHandTypeChanged.AddDynamic(this, &UStatsManagerComponent::OnMainHandTypeSwitched);
    }
}

void UStatsManagerComponent::ExcludeItemModifiers(TSubclassOf<UItemBase> Item)
{
    if (UKismetSystemLibrary::IsValidClass(Item))
    {
        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Item);

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

void UStatsManagerComponent::IncludeItemModifiers(TSubclassOf<UItemBase> Item)
{
    if (UKismetSystemLibrary::IsValidClass(Item))
    {
        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Item);

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

void UStatsManagerComponent::AddModifier(EStat Type, float Value)
{
    if (Value != 0.0f)
    {
        int Index = GetStatIndex(Type);

        if (Index >= 0)
        {
            Stats[Index].ModifiersValue += Value;
            OnModifierAdded.Broadcast(Type, Value);
        }
    }
}

void UStatsManagerComponent::RemoveModifier(EStat Type, float Value)
{
    if (Value != 0.0f)
    {
        int Index = GetStatIndex(Type);

        if (Index >= 0)
        {
            Stats[Index].ModifiersValue -= Value;

            OnModifierRemoved.Broadcast(Type, Value);
        }
    }
}

float UStatsManagerComponent::GetStatValue(EStat Type, bool bIncludeModifiers) const
{
    int Index = GetStatIndex(Type);

    if (Index >= 0)
    {
        float BaseValue = Stats[Index].BaseValue;
        float ModifierValue = Stats[Index].ModifiersValue;

        if (bIncludeModifiers)
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

int UStatsManagerComponent::GetStatIndex(EStat Type) const
{
    for (int i = 0; i < Stats.Num(); ++i)
    {
        if (Stats[i].Type == Type)
        {
            return i;
        }
    }

    return -1;
}

float UStatsManagerComponent::GetDamage() const
{
    float Damage = GetStatValue(EStat::Damage, true);

    if (UKismetMathLibrary::RandomFloatInRange(1.0f, 100.0f) <= GetStatValue(EStat::CritChance, true))
    {
        Damage *= GetStatValue(EStat::CritMultiplier, true);
    }

    return UKismetMathLibrary::FTrunc(Damage);
}

void UStatsManagerComponent::ChangeStatBaseValue(EStat Type, float NewValue)
{
    int Index = GetStatIndex(Type);
    Stats[Index].BaseValue = NewValue;

    OnBaseValueChanged.Broadcast(Type, NewValue);
}

void UStatsManagerComponent::TakeDamage(float Damage, bool bIgnoreStamina)
{
    GetWorld()->GetTimerManager().ClearTimer(ResetRecentlyReceivedDamageTimerHandle);

    RecentlyReceivedDamage += Damage;
    RecentlyReceivedHitsCount++;

    GetWorld()->GetTimerManager().SetTimer(
        ResetRecentlyReceivedDamageTimerHandle, this, &UStatsManagerComponent::ResetRecentlyReceivedDamage, 4.0f, false);
    
    UExtendedStatComponent* HealthStatComp = UDefaultGameInstance::GetExtendedStatComponent(GetOwner(), EStat::Health);
    UExtendedStatComponent* StaminaStatComp = UDefaultGameInstance::GetExtendedStatComponent(GetOwner(), EStat::Stamina);

    float ArmorVal = Damage * (GetStatValue(EStat::Armor, true) / 100.0f);
    float ReducedDamage = FMath::Clamp(Damage - ArmorVal, 0.0f, Damage - ArmorVal);
        
    if (HealthStatComp->IsValidLowLevel())
    {
        if (bIgnoreStamina || !StaminaStatComp->IsValidLowLevel())
        {
            HealthStatComp->ModifyStat(ReducedDamage * -1.0f, true);

            // ???
            RecentlyReceivedSuccessfulDamage += Damage;
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

                RecentlyReceivedSuccessfulDamage += Damage;
                RecentlyReceivedSuccessfulHitsCount++;
            }

        }
    }        
    



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
