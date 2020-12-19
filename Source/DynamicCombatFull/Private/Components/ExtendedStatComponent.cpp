// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "GameCore/DCSGameMode.h"
#include "Components/StatsManagerComponent.h"


// Sets default values for this component's properties
UExtendedStatComponent::UExtendedStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
	
    RegenerationTickInterval = 0.05f;
    CurrentValue = 100.0f;
    TopValue = 100.0f;
    RegenValue = 1.0f;
    ReenableRegenTime = 0.5f;
}


// Called when the game starts
void UExtendedStatComponent::BeginPlay()
{
	Super::BeginPlay();

    InitStatManager();

    InitialRegenValue = RegenValue;
    SetCurrentValue(TopValue, false);

    RefreshRegenTimer();
    
    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        GameMode->OnGameLoaded.AddDynamic(this, &UExtendedStatComponent::OnGameLoaded);
    }

}


void UExtendedStatComponent::RefreshRegenTimer()
{
    if (bDoesRegenerates)
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
        {
            GetWorld()->GetTimerManager().SetTimer(
                RegenTimerHandle, this, &UExtendedStatComponent::StartRegenerating, ReenableRegenTime, false);
        }
    }
}

void UExtendedStatComponent::StartRegenerating()
{
    FTimerHandle TimerHandle;

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &UExtendedStatComponent::RegenTick, RegenerationTickInterval, true);
}

void UExtendedStatComponent::OnModifierAdded(EStat Type, float Value)
{
    if (Type == StatType)
    {
        AddModifier(Value);
    }
}

void UExtendedStatComponent::OnModifierRemoved(EStat Type, float Value)
{
    if (Type == StatType)
    {
        RemoveModifier(Value);
    }
}

void UExtendedStatComponent::InitStatManager()
{
    UStatsManagerComponent* StatsManagerComp = 
        Cast<UStatsManagerComponent>(GetOwner()->GetComponentByClass(UStatsManagerComponent::StaticClass()));

    if (StatsManagerComp->IsValidLowLevel())
    {
        float StatValue = StatsManagerComp->GetStatValue(StatType, false);
        TopValue = StatValue;

        StatsManagerComp->OnModifierAdded.AddDynamic(this, &UExtendedStatComponent::OnModifierAdded);
        StatsManagerComp->OnModifierRemoved.AddDynamic(this, &UExtendedStatComponent::OnModifierRemoved);
        StatsManagerComp->OnBaseValueChanged.AddDynamic(this, &UExtendedStatComponent::OnBaseValueChanged);
    }
}

void UExtendedStatComponent::OnGameLoaded()
{
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &UExtendedStatComponent::OnGameLoadedDelayed, 0.0f, false);
}

void UExtendedStatComponent::OnGameLoadedDelayed()
{
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode != nullptr)
    {
        const TMap<EStat, float>& StatValues = GameMode->GetCurrentStatValues();

        if (StatValues.Contains(StatType))
        {
            float StatValue = StatValues[StatType];
            SetCurrentValue(StatValue, false);
        }
    }
}

void UExtendedStatComponent::OnBaseValueChanged(EStat Stat, float NewValue)
{
    if (Stat == StatType)
    {
        TopValue = NewValue;

        OnValueChanged.Broadcast(CurrentValue, GetMaxValue());
    }
}

void UExtendedStatComponent::RegenTick()
{
    if (CurrentValue < GetMaxValue())
    {
        ModifyStat(RegenValue, false);
    }
    else
    {
        ClearRegenTimer();
    }
}

void UExtendedStatComponent::ClearRegenTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
}

void UExtendedStatComponent::ChangeRegenPercent(int Percent)
{
    RegenValue = ((float)Percent / 100.0f) * InitialRegenValue;
}

void UExtendedStatComponent::ModifyStat(float Value, bool bInterruptRegeneration)
{
    if (Value != 0.0f)
    {
        if (bInterruptRegeneration)
        {
            ClearRegenTimer();
        }

        SetCurrentValue(CurrentValue + Value, false);
        RefreshRegenTimer();
    }
}


void UExtendedStatComponent::AddModifier(float Value)
{
    ModifierValue = ModifierValue + Value;

    SetCurrentValue(FMath::Clamp(CurrentValue, 0.0f, GetMaxValue()), false);
    RefreshRegenTimer();
}

void UExtendedStatComponent::RemoveModifier(float Value)
{
    ModifierValue = ModifierValue - Value;

    SetCurrentValue(FMath::Clamp(CurrentValue, 0.0f, GetMaxValue()), false);
    RefreshRegenTimer();
}

void UExtendedStatComponent::SetCurrentValue(float Value, bool bInterruptRegeneration)
{
    CurrentValue = FMath::Clamp(Value, -100.0f, GetMaxValue());

    OnValueChanged.Broadcast(CurrentValue, GetMaxValue());

    if (bInterruptRegeneration)
    {
        ClearRegenTimer();
        RefreshRegenTimer();
    }
}


