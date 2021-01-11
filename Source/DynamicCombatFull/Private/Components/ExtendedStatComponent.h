// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "ExtendedStatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValueChangedSignature, float, InNewValue, float, InMaxValue);

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
    UFUNCTION(BlueprintCallable)
    void ChangeRegenPercent(int InPercent);

    UFUNCTION(BlueprintCallable)
    void ModifyStat(float InValue, bool bInInterruptRegeneration);

public:


    float GetCurrentValue() const { return CurrentValue; }
    void SetCurrentValue(float InValue, bool bInInterruptRegeneration);

    EStat GetStatType() const { return StatType; }
    void SetStatType(EStat InStatType) { StatType = InStatType; }

    void SetDoesRegenerates(bool bInDoesRegenerates) { bDoesRegenerates = bInDoesRegenerates; }
    void SetRegenValue(float InRegenValue) { RegenValue = InRegenValue; }
    void SetReenableRegenTime(float InReenableRegenTime) { ReenableRegenTime = InReenableRegenTime; }
    float GetMaxValue() const { return TopValue + ModifierValue; }

protected:
    UFUNCTION()
    void OnModifierAdded(EStat InType, float InValue);

    UFUNCTION()
    void OnModifierRemoved(EStat InType, float InValue);

    UFUNCTION()
    void OnGameLoaded();

    UFUNCTION()
    void OnBaseValueChanged(EStat InStat, float InNewValue);

private:
    void RefreshRegenTimer();
    void StartRegenerating();
    void InitStatManager();
    void DelayedTick_OnGameLoaded();
    void RegenTick();
    void ClearRegenTimer();
    void AddModifier(float InValue);
    void RemoveModifier(float InValue);


public:
    UPROPERTY(BlueprintAssignable)
    FValueChangedSignature OnValueChanged;

private:
    UPROPERTY(EditAnywhere)
    EStat StatType;

    UPROPERTY(EditAnywhere)
    bool bDoesRegenerates;

    UPROPERTY(EditAnywhere)
    float RegenValue;

    UPROPERTY(EditAnywhere)
    float ReenableRegenTime;

    FTimerHandle RegenTimerHandle;
    float InitialRegenValue;

    float RegenerationTickInterval;
    float CurrentValue;
    float TopValue;
    float ModifierValue;
};
