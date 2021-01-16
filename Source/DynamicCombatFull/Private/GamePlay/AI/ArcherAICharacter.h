// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/AI/AICharacter.h"
#include "Interfaces/IsArcher.h"
#include "ArcherAICharacter.generated.h"

class UDataTable;
class USceneComponent;
/**
 * 
 */
UCLASS()
class AArcherAICharacter : public AAICharacter, public IIsArcher
{
	GENERATED_BODY()
public:
    // Sets default values for this character's properties
    AArcherAICharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void OnConstruction(const FTransform& Transform) override;

    void UpdateAimAlpha(float DeltaTime);

public:
    // IIsArcher
    virtual ACharacter* GetThisCharacter() override { return this; }
    virtual UEquipmentComponent* GetEquipment() const override { return Equipment; }
    virtual UStatsManagerComponent* GetStatsManager() const override { return StatsManager; }

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "GetAimAlpha", ScriptName = "GetAimAlpha"))
        float K2_GetAimAlpha() const;

    virtual float K2_GetAimAlpha_Implementation() const { return GetAimAlpha(); }

    virtual float GetAimAlpha() const override { return AimAlpha; }
    virtual float GetArrowInitialSpeed() const override { return ArrowInitialSpeed; }
    virtual bool DoesHoldBowString() const override;
    virtual float BowAttack() override;
    virtual FTransform GetSpawnArrowTransform() const override;

protected:
    UFUNCTION()
    void OnActivityChanged(EActivity InActivity, bool bValue);

private:
    UPROPERTY(EditAnywhere)
    float ArrowInitialSpeed;
    float AimAlpha;

    UPROPERTY(EditAnywhere)
    USceneComponent* ArrowSpawnLocation;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    USoundBase* DefaultDrawBowSoundObject;
};
