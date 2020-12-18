// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Abilities/PlayerAbilityBase.h"
#include "PlayerSummonItemAbilityBase.generated.h"

class ASummonedItemAbilityEffect;
class USoundBase;
class UParticleSystem;
class UItemBase;
/**
 * 
 */
UCLASS()
class APlayerSummonItemAbilityBase : public APlayerAbilityBase
{
	GENERATED_BODY()
	
public:
    APlayerSummonItemAbilityBase();

    virtual void Effect() override;
    virtual void Released() override;
    void SpawnParticle();
    void Summon();
    void PlaySound();

    void SpawnItem(TSubclassOf<UItemBase> Class);

private:

    UPROPERTY(EditAnywhere)
    TSubclassOf<ASummonedItemAbilityEffect> SpawnSummonedItemAbilityEffectClass;

    TArray<TSubclassOf<UItemBase>> SummonedItemClasses;

    float Duration;

    bool bAutoSwap;

    UPROPERTY()
        UParticleSystem* SummonCastParticle;

    UPROPERTY()
        USoundBase* Sound;


};
