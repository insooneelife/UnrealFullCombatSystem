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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    void RefreshRegenTimer();

    void StartRegenerating();

    UFUNCTION()
    void OnModifierAdded(EStat Type, float Value);

    UFUNCTION()
    void OnModifierRemoved(EStat Type, float Value);

    void InitStatManager();

    UFUNCTION()
    void OnGameLoaded();

    void OnGameLoadedDelayed();

    UFUNCTION()
    void OnBaseValueChanged(EStat Stat, float NewValue);

    void RegenTick();

    void ClearRegenTimer();

    UFUNCTION(BlueprintCallable)
    void ChangeRegenPercent(int Percent);

    UFUNCTION(BlueprintCallable)
    void ModifyStat(float Value, bool bInterruptRegeneration);

    void AddModifier(float Value);
    void RemoveModifier(float Value);

    void SetCurrentValue(float Value, bool bInterruptRegeneration);

public:
    EStat GetStatType() const { return StatType; }
    void SetStatType(EStat InStatType) { StatType = InStatType; }

    void SetDoesRegenerates(bool bInDoesRegenerates) { bDoesRegenerates = bInDoesRegenerates; }
    void SetRegenValue(float InRegenValue) { RegenValue = InRegenValue; }
    void SetReenableRegenTime(float InReenableRegenTime) { ReenableRegenTime = InReenableRegenTime; }

    float GetCurrentValue() const { return CurrentValue; }
    float GetMaxValue() const { return TopValue + ModifierValue; }

public:
    UPROPERTY(BlueprintAssignable)
    FValueChangedSignature OnValueChanged;

private:
    UPROPERTY(EditAnywhere)
    EStat StatType;

    float RegenerationTickInterval;
    float CurrentValue;
    float TopValue;
    float ModifierValue;

    UPROPERTY(EditAnywhere)
    bool bDoesRegenerates;

    UPROPERTY(EditAnywhere)
    float RegenValue;

    UPROPERTY(EditAnywhere)
    float ReenableRegenTime;

    FTimerHandle RegenTimerHandle;
    float InitialRegenValue;
};
