// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "Interfaces/IsTargetable.h"
#include "Interfaces/CanBeAttacked.h"
#include "Interfaces/CanGetEffects.h"
#include "Interfaces/MontageManagerInterface.h"
#include "Interfaces/RotatingInterface.h"
#include "Interfaces/CanMeleeAttack.h"
#include "Interfaces/MontageAction.h"
#include "AICharacter.generated.h"

class UAnimMontage;
class UBehaviorTree;
class ABaseAIController;
class UStatBarUI;
class UWidgetComponent;
class UMovementSpeedComponent;
class UPatrolComponent;
class UStateManagerComponent;
class UStatsManagerComponent;
class URotatingComponent;
class UExtendedStatComponent;
class UEquipmentComponent;
class UDissolveComponent;
class UEffectsComponent;
class UBehaviorComponent;
class UMontageManagerComponent;
class UCollisionHandlerComponent;


UCLASS()
class AAICharacter 
    : 
    public ACharacter, 
    public IIsTargetable, 
    public ICanBeAttacked, 
    public ICanGetEffects, 
    public IMontageManagerInterface,
    public IRotatingInterface,
    public ICanMeleeAttack,
    public IMontageAction
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

public:
    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(BlueprintCallable)
        ABaseAIController* GetAIController() const { return AIController; }

    UFUNCTION(BlueprintCallable)
        EState GetState() const;

public:
    UBehaviorTree* GetBTree() const { return BTree; }



    UPatrolComponent* GetPatrol() const { return Patrol; }

    UExtendedStatComponent* GetExtendedStamina() const { return ExtendedStamina; }

protected:

    UFUNCTION()
        void OnEffectApplied(EEffectType InType);

    UFUNCTION()
        void OnEffectRemoved(EEffectType InType);

    void HandleMeshOnDeath();

    void Delayed_HandleMeshOnDeath();

    UFUNCTION(BlueprintCallable)
        void OnValueChanged_ExtendedHealth(float InNewValue, float InMaxValue);

    void InitializeStatsWidget();

public:	

    void ShowStatsWidget();
    void HideStatsWidget();

    void Death();
    void Stunned();
    void Parried();
    void Impact();
    void Backstabbed();

    void PlayEffectMontage(UAnimMontage* AnimMontage, EEffectType EffectType);

    float Roll(EDirection InDirection);

public:
    bool IsStateEqualPure(EState InState) const;
    bool IsActivityPure(EActivity InActivity) const;
    bool IsCombatTypePure(ECombatType InType) const;

public:
    // IIsTargetable
    virtual void OnSelected() override;
    virtual void OnDeselected() override;
    virtual bool IsTargetable() const override;

    // ICanBeAttacked
    virtual UStateManagerComponent* GetStateManager() const override { return StateManager; }
    virtual UStatsManagerComponent* GetStatsManager() const override { return StatsManager; }
    virtual UExtendedStatComponent* GetExtendedStaminaComp() const override { return ExtendedStamina; }

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TakeDamage", ScriptName = "TakeDamage"))
        bool K2_TakeDamage(const FHitData& InHitData, EAttackResult& OutResult);

    virtual bool K2_TakeDamage_Implementation(const FHitData& InHitData, EAttackResult& OutResult) override
    {
        return ICanBeAttacked::TakeDamage(InHitData, OutResult);
    }

    virtual void ReportDamage(const FHitData& InHitData) override;

    virtual bool IsAlive() const override;
    virtual FName GetHeadSocket() const override;
    virtual bool CanBeAttacked() const override;
    virtual void Block() override;
    virtual bool IsBlocked() const override;
    virtual void UpdateReceivedHitDirection(FVector InHitFromDirection) override;
    virtual EDirection GetReceivedHitDirection() const override { return ReceivedHitDirection; }

    // ICanGetEffects
    virtual bool CanBeStunned() const override;
    virtual bool CanBeInterrupted() const override;
    virtual bool CanBeBackstabbed() const override;

    // IRotatingInterface
    virtual FRotator GetDesiredRotation() const override;

    // IMontageManagerInterface
    virtual UDataTable* GetMontages(EMontageAction InAction) const override;

    // ICanMeleeAttack
    virtual ACharacter* GetThisCharacter() override { return this; }
    virtual UCollisionHandlerComponent* GetMeleeCollisionHandler() const override { return MeleeCollisionHandler; }
    virtual UEquipmentComponent* GetEquipment() const override { return Equipment; }
    virtual EMeleeAttackType GetMeleeAttackType() const override { return MeleeAttackType; }
    virtual void SetMeleeAttackCounter(int Value) override { MeleeAttackCounter = Value; }
    virtual int GetMeleeAttackCounter()const override { return MeleeAttackCounter; }
    virtual bool CanMeleeAttack() const override { return true; }
    virtual float GetMeleeDamage() const override;
    virtual float MeleeAttack(EMeleeAttackType InType) override;

    virtual const TArray<FName>& GetLeftHandCollisionSockets() const override { return LeftHandCollisionSockets; }
    virtual const TArray<FName>& GetRightHandCollisionSockets() const override { return RightHandCollisionSockets; }
    virtual const TArray<FName>& GetRightFootCollisionSockets() const override { return RightFootCollisionSockets; }
    virtual const TArray<FName>& GetLeftFootCollisionSockets() const override { return LeftFootCollisionSockets; }

    // IMontageAction
    virtual UMontageManagerComponent* GetMontageManager() const override { return MontageManager; }

public:

    UAnimMontage* GetRollMontage(EDirection InDirection) const;

private:
    void SetData();

protected:
    UPROPERTY(EditAnywhere, Category = "Components")
    UMovementSpeedComponent* MovementSpeed;

    UPROPERTY(EditAnywhere, Category = "Components")
    UPatrolComponent* Patrol;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStateManagerComponent* StateManager;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStatsManagerComponent* StatsManager;

    UPROPERTY(EditAnywhere, Category = "Components")
    URotatingComponent* Rotating;

    UPROPERTY(EditAnywhere, Category = "Components")
    UExtendedStatComponent* ExtendedStamina;

    UPROPERTY(EditAnywhere, Category = "Components")
    UEquipmentComponent* Equipment;

    UPROPERTY(EditAnywhere, Category = "Components")
    UDissolveComponent* Dissolve;

    UPROPERTY(EditAnywhere, Category = "Components")
    UEffectsComponent* Effects;

    UPROPERTY(EditAnywhere, Category = "Components")
    UBehaviorComponent* Behavior;

    UPROPERTY(EditAnywhere, Category = "Components")
    UMontageManagerComponent* MontageManager;

    UPROPERTY(EditAnywhere, Category = "Components")
    UExtendedStatComponent* ExtendedHealth;

    UPROPERTY(EditAnywhere, Category = "Components")
    UCollisionHandlerComponent* MeleeCollisionHandler;

    // find from hierachy
    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    UWidgetComponent* TargetWidget;

    // find from hierachy
    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    UWidgetComponent* StatBarsWidget;

    // find in beginplay
    UPROPERTY()
    ABaseAIController* AIController;


    UPROPERTY()
    TArray<AActor*> AttachedActors;

    UPROPERTY(EditAnywhere, Category = "Combat")
    EDirection ReceivedHitDirection;

    UPROPERTY(EditAnywhere, Category = "Combat")
    EMeleeAttackType MeleeAttackType;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int MeleeAttackCounter;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> RightHandCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> LeftHandCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> RightFootCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> LeftFootCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float RecentlyReceivedDamageStunLimit;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int RecentlyReceivedHitsCountStunLimit;

    UPROPERTY(EditAnywhere, Category = "AI")
    UBehaviorTree* BTree;

    FTimerHandle ResetMeleeAttackCounterTimerHandle;
};
