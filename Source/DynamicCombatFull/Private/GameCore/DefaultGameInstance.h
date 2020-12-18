// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundBase.h"
#include "GameCore/DataTypes.h"
#include "DefaultGameInstance.generated.h"


class UExtendedStatComponent;
/**
 *
 */
UCLASS()
class UDefaultGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    static const FName Inventory;
    static const FName UIBack;
    static const FName UITakeAll;
    static const FName Equipment;
    static const FName Interact;
    static const FName TakeAll;
    static const FName Back;


public:
    UDefaultGameInstance(const FObjectInitializer& ObjectInitializer);

    void PlayHitSound(AActor* Applier, AActor* Receiver, FVector Location);

    void PlayParrySound(AActor* Applier, AActor* Receiver, FVector Location);

    void PlayBlockSound(AActor* Applier, AActor* Receiver, FVector Location);

    static EMontageAction ConvertMeleeAttackTypeToAction(EMeleeAttackType Type);

    static UExtendedStatComponent* GetExtendedStatComponent(AActor* Actor, EStat Type);

    static TArray<AActor*> SelectEnemyActors(AActor* Target, const TArray<AActor*>& Actors);

    static AActor* GetClosestActor(AActor* Target, const TArray<AActor*>& Actors);

    static FKey GetFirstActionMappingKey(FName ActionName);

    static EDirection GetHitDirection(FVector HitFromDirection, AActor* AttackedActor);

    static float ScaleMeleeAttackStaminaCostByType(FVector Cost, EMeleeAttackType MeleeAttackType);

    static float ScaleMeleeDamageByType(float Damage, EMeleeAttackType MeleeAttackType);

public:
    UPROPERTY()
        USoundBase* DefaultHitSound;

    UPROPERTY()
        USoundBase* SwordHitSound;

    UPROPERTY()
        USoundBase* AxeHitSound;

    UPROPERTY()
        USoundBase* BlockShieldSound;
};