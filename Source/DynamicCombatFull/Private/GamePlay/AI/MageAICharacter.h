// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/AI/AICharacter.h"
#include "Interfaces/AbilityInterface.h"
#include "MageAICharacter.generated.h"


class UDataTable;
class USceneComponent;
class UAbilityComponent;
/**
 * 
 */
UCLASS()
class AMageAICharacter : public AAICharacter, public IAbilityInterface
{
	GENERATED_BODY()
public:
    // Sets default values for this character's properties
    AMageAICharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

public:
    UFUNCTION(BlueprintCallable)
    void MakeUntargetable(float InDuration);

    UFUNCTION(BlueprintCallable)
    float GetCurrentMana() const; 

public:
    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

    // IMontageManagerInterface
    virtual UDataTable* GetMontages(EMontageAction InAction) const override;

    // IRotatingInterface
    virtual FRotator GetDesiredRotation() const override;

    // IIsTargetable
    virtual bool IsTargetable() const override;

    // IAbilityInterface
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "CanCastAbility", ScriptName = "CanCastAbility"))
    bool K2_CanCastAbility() const;

    virtual bool K2_CanCastAbility_Implementation() const override { return CanCastAbility(); }

    virtual bool CanCastAbility() const override;
    virtual float GetMagicDamage() const override;
    virtual float GetCastingSpeed() const override;

protected:
    UFUNCTION()
    void OnAbilityStarted();

    UFUNCTION()
    void OnAbilityEnded(EAbilityEndResult InEndResult);

    UFUNCTION()
    void OnManaConsumed(float InAmount);

    UFUNCTION()
    void OnStateChanged(EState InPrevState, EState InNewState);

private:

    void Delayed_Set();
    bool IsStateEqual(EState InState);

private:

    UPROPERTY(EditAnywhere, Category = "Components")
        UAbilityComponent* AbilityComponent;

    UPROPERTY(EditAnywhere, Category = "Components")
        UExtendedStatComponent* ExtendedMana;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
        UDataTable* AIMageMontages;

    bool bIsTargetable;
    FTimerHandle RetriggerableDelayTimerHandle;
};
