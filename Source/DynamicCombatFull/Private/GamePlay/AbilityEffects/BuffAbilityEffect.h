// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "BuffAbilityEffect.generated.h"

class UStatsManagerComponent;
class UParticleSystemComponent;

UCLASS()
class ABuffAbilityEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuffAbilityEffect();

protected:

public:
    void Init(float InDuration, EStat InStatType, float InValue, FLinearColor InColor);

    void ApplyBuff();
    void AttachToOwner();
    void UpdateParticleColor();
    void UpdateElapsedTime();
    void StartTimer();
    void Deactivate();
    void RemoveBuff();
    void AdjustBuff(float NewValue, float NewDuration);

public:
    float GetDuration() const { return Duration; }
    EStat GetStatType() const { return StatType; }
    float GetValue() const { return Value; }
    FLinearColor GetColor() const { return Color; }
    float GetElapsedTime() const { return ElapsedTime; }
    FName GetAttachmentSocket() const { return AttachmentSocket; }
    float GetTimerInterval() const { return TimerInterval; }

public:	

    UPROPERTY()
    UStatsManagerComponent* StatsManagerComponent;

    UPROPERTY()
    UParticleSystemComponent* ParticleSystemComponent;

    UPROPERTY(EditAnywhere)
    float Duration;

    UPROPERTY(EditAnywhere)
    EStat StatType;

    UPROPERTY(EditAnywhere)
    float Value;

    UPROPERTY(EditAnywhere)
    FLinearColor Color;

    UPROPERTY(EditAnywhere)
    float ElapsedTime;

    UPROPERTY(EditAnywhere)
    FName AttachmentSocket;

    UPROPERTY(EditAnywhere)
    float TimerInterval;

    FTimerHandle UpdateElapsedTimeTimerHandle;
};
