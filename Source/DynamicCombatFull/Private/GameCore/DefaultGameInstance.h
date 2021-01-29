// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundBase.h"
#include "GameCore/DataTypes.h"
#include "DefaultGameInstance.generated.h"


class UExtendedStatComponent;
class UParticleSystem;
class USoundBase;
class UMaterialInstance;
class APickupActor;
class UKeybindingsUI;
/**
 *
 */
UCLASS()
class UDefaultGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UDefaultGameInstance();

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

    virtual void Shutdown() override;

public:
    void PlayHitSound(AActor* Applier, AActor* Receiver, FVector Location);

    void PlayParrySound(AActor* Applier, AActor* Receiver, FVector Location);

    void PlayBlockSound(AActor* Applier, AActor* Receiver, FVector Location);

public:



    static EMontageAction ConvertMeleeAttackTypeToAction(EMeleeAttackType Type);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = DefaultGameInstance)
    static UExtendedStatComponent* GetExtendedStatComponent(AActor* Actor, EStat Type);

    static TArray<AActor*> SelectEnemyActors(AActor* Target, const TArray<AActor*>& Actors);

    static AActor* GetClosestActor(AActor* Target, const TArray<AActor*>& Actors);

    static FKey GetFirstActionMappingKey(FName ActionName);

    static EDirection GetHitDirection(FVector HitFromDirection, AActor* AttackedActor);

    static float ScaleMeleeAttackStaminaCostByType(float Cost, EMeleeAttackType MeleeAttackType);

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

    UPROPERTY()
    UParticleSystem* ExplosionParticle;

    UPROPERTY()
    USoundBase* GroundExplosionSound;

    UPROPERTY()
    TSubclassOf<class ASpellIndicatorActor> SpellIndicatorClass;

    UPROPERTY()
    UMaterialInstance* DissolveMaterial;

    UPROPERTY()
    TSubclassOf<AActor> SpawnPickupActorClass;

    UPROPERTY()
    UTexture2D* DefaultCrosshairTextureObject;

    UPROPERTY()
    UTexture2D* CrosshairTexture;

    UPROPERTY()
    TSubclassOf<UUserWidget> InGameUIClass;

    UPROPERTY()
    TSubclassOf<UKeybindingsUI> KeybindingsUIClass;
};