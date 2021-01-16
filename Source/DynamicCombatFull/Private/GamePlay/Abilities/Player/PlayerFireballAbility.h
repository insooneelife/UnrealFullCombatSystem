// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerFireballAbility.generated.h"

class AFireballProjectileAbilityEffect;
/**
 * 
 */
UCLASS()
class APlayerFireballAbility : public APlayerAbilityBase
{
	GENERATED_BODY()
	
public:
    APlayerFireballAbility();

public:
    virtual void Released() override;
    virtual void Effect() override;

    void ResetMontageCounter();
    void UpdateMontageCounter();
    void UpdateSocket();

private:
    void Delayed_Released();

private:
    
    int MontageCounter;
    bool bUseLeftHandSocket;

    UPROPERTY()
    float InitialProjectileSpeed;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AFireballProjectileAbilityEffect*> FireballProjectileClass;

    FTimerHandle RetriggerableTimerHandle;
};
