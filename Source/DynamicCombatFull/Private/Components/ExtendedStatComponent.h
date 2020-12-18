// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "ExtendedStatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValueChangedSignature, float, NewValue, float, MaxValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UExtendedStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExtendedStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
    void RefreshRegenTimer();

    UFUNCTION()
    void StartRegenerating();

    void OnModifierAdded(EStat Type, float Value);
    void OnModifierRemoved(EStat Type, float Value);

    void InitStatManager();

    UFUNCTION()
    void OnGameLoaded();
    void OnGameLoadedDelayed();
    void OnBaseValueChanged(EStat Stat, float NewValue);

    UFUNCTION()
    void RegenTick();

    void ClearRegenTimer();
    void ChangeRegenPercent(int Percent);

    void ModifyStat(float Value, bool bInterruptRegeneration);

    void AddModifier(float Value);
    void RemoveModifier(float Value);

    void SetCurrentValue(float Value, bool bInterruptRegeneration);

public:
    EStat GetStatType() const { return StatType; }

    float GetCurrentValue() const { return CurrentValue; }

    float GetMaxValue() const { return TopValue + ModifierValue; }

public:
    UPROPERTY(BlueprintAssignable)
    FValueChangedSignature OnValueChanged;

private:
    EStat StatType;
    float RegenerationTickInterval;
    float CurrentValue;
    float TopValue;
    float ModifierValue;
    bool bDoesRegenerates;
    float RegenValue;
    float ReenableRegenTime;
    FTimerHandle RegenHandle;
    float InitialRegenValue;
};
