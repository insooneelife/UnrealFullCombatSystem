// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "GameCore/DCSGameMode.h"
#include "GameCore/GameUtils.h"
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

        if (GameUtils::IsValid(GameMode))
        {
            GameMode->OnGameLoaded.AddDynamic(this, &UExtendedStatComponent::OnGameLoaded);
        }
    }
}

void UExtendedStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (GameUtils::IsValid(GameMode))
        {
            GameMode->OnGameLoaded.RemoveDynamic(this, &UExtendedStatComponent::OnGameLoaded);
        }
    }

    UStatsManagerComponent* StatsManagerComp =
        Cast<UStatsManagerComponent>(GetOwner()->GetComponentByClass(UStatsManagerComponent::StaticClass()));

    if (GameUtils::IsValid(StatsManagerComp))
    {
        StatsManagerComp->OnModifierAdded.RemoveDynamic(this, &UExtendedStatComponent::OnModifierAdded);
        StatsManagerComp->OnModifierRemoved.RemoveDynamic(this, &UExtendedStatComponent::OnModifierRemoved);
        StatsManagerComp->OnBaseValueChanged.RemoveDynamic(this, &UExtendedStatComponent::OnBaseValueChanged);
    }
}

void UExtendedStatComponent::ChangeRegenPercent(int InPercent)
{
    RegenValue = ((float)InPercent / 100.0f) * InitialRegenValue;
}

void UExtendedStatComponent::ModifyStat(float InValue, bool bInInterruptRegeneration)
{
    if (InValue != 0.0f)
    {
        if (bInInterruptRegeneration)
        {
            ClearRegenTimer();
        }

        SetCurrentValue(CurrentValue + InValue, false);
        RefreshRegenTimer();
    }
}

void UExtendedStatComponent::SetCurrentValue(float InValue, bool bInInterruptRegeneration)
{
    CurrentValue = FMath::Clamp(InValue, -100.0f, GetMaxValue());

    OnValueChanged.Broadcast(CurrentValue, GetMaxValue());

    if (bInInterruptRegeneration)
    {
        ClearRegenTimer();
        RefreshRegenTimer();
    }
}

void UExtendedStatComponent::OnModifierAdded(EStat InType, float InValue)
{
    if (InType == StatType)
    {
        AddModifier(InValue);
    }
}

void UExtendedStatComponent::OnModifierRemoved(EStat InType, float InValue)
{
    if (InType == StatType)
    {
        RemoveModifier(InValue);
    }
}

void UExtendedStatComponent::OnGameLoaded()
{
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &UExtendedStatComponent::DelayedTick_OnGameLoaded, 0.0f, false);
}

void UExtendedStatComponent::OnBaseValueChanged(EStat InStat, float InNewValue)
{
    if (InStat == StatType)
    {
        TopValue = InNewValue;

        OnValueChanged.Broadcast(CurrentValue, GetMaxValue());
    }
}

void UExtendedStatComponent::RefreshRegenTimer()
{
    if (bDoesRegenerates)
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle, this, &UExtendedStatComponent::StartRegenerating, ReenableRegenTime, false);
        }
    }
}

void UExtendedStatComponent::StartRegenerating()
{
    GetWorld()->GetTimerManager().SetTimer(
        RegenTimerHandle, this, &UExtendedStatComponent::RegenTick, RegenerationTickInterval, true);
}

void UExtendedStatComponent::InitStatManager()
{
    UStatsManagerComponent* StatsManagerComp = 
        Cast<UStatsManagerComponent>(GetOwner()->GetComponentByClass(UStatsManagerComponent::StaticClass()));

    if (GameUtils::IsValid(StatsManagerComp))
    {
        float StatValue = StatsManagerComp->GetStatValue(StatType, false);
        TopValue = StatValue;

        StatsManagerComp->OnModifierAdded.AddDynamic(this, &UExtendedStatComponent::OnModifierAdded);
        StatsManagerComp->OnModifierRemoved.AddDynamic(this, &UExtendedStatComponent::OnModifierRemoved);
        StatsManagerComp->OnBaseValueChanged.AddDynamic(this, &UExtendedStatComponent::OnBaseValueChanged);
    }
}

void UExtendedStatComponent::DelayedTick_OnGameLoaded()
{
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameUtils::IsValid(GameMode))
    {
        const TMap<EStat, float>& StatValues = GameMode->GetCurrentStatValues();

        if (StatValues.Contains(StatType))
        {
            float StatValue = StatValues[StatType];
            SetCurrentValue(StatValue, false);
        }
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

void UExtendedStatComponent::AddModifier(float InValue)
{
    ModifierValue = ModifierValue + InValue;

    SetCurrentValue(FMath::Clamp(CurrentValue, 0.0f, GetMaxValue()), false);
    RefreshRegenTimer();
}

void UExtendedStatComponent::RemoveModifier(float InValue)
{
    ModifierValue = ModifierValue - InValue;

    SetCurrentValue(FMath::Clamp(CurrentValue, 0.0f, GetMaxValue()), false);
    RefreshRegenTimer();
}



