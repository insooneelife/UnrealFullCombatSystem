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
    public IRotatingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
        void OnEffectApplied(EEffectType InType);

    UFUNCTION()
        void OnEffectRemoved(EEffectType InType);

    // weapon collision events
    UFUNCTION()
        void OnHit(const FHitResult& InHit);

    UFUNCTION()
        void OnCollisionActivated(ECollisionPart InCollisionPart);

    void HandleMeshOnDeath();

    UFUNCTION(BlueprintCallable)
        void OnValueChanged_ExtendedHealth(float InNewValue, float InMaxValue);

    void InitializeStatsWidget();

public:	
    UAnimMontage* GetStunMontage(EDirection InDirection) const;

    UAnimMontage* GetBlockMontage() const;
    UAnimMontage* GetImpactMontage() const;
    UAnimMontage* GetParriedMontage() const;
    UAnimMontage* GetRollMontage() const;

    bool CanBeStunned() const;
    bool CanBeAttacked() const;
    bool CanBeBackstabbed() const;

    void ShowStatsWidget();
    void HideStatsWidget();

    void Death();
    void Stunned();
    void Block();
    void Parried();
    void Impact();
    void Backstabbed();

    FHitData MakeMeleeHitData(AActor* InHitActor);
    void ApplyHitImpulseToCharacter(AActor* InActor, FVector InHitNormal, float InImpulsePower);

    float MeleeAttack(EMeleeAttackType InType);
    UAnimMontage* GetMeleeAttackMontage() const;
    void ResetMeleeAttackCounter();
    float Roll(EDirection InDirection);

public:
    bool IsStateEqualPure(EState InState) const;
    bool IsActivityPure(EActivity InActivity) const;
    bool IsCombatTypePure(ECombatType InType) const;
    void UpdateReceivedHitDirection(float InHitFromDirection);
    bool CanBeInterrupted() const;

public:
    // IIsTargetable
    virtual void OnSelected() override;
    virtual void OnDeselected() override;
    virtual bool IsTargetable() const override;

    // ICanBeAttacked
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TakeDamage", ScriptName = "TakeDamage"))
        bool K2_TakeDamage(const FHitData& InHitData, EAttackResult& OutResult);

    virtual bool K2_TakeDamage_Implementation(const FHitData& InHitData, EAttackResult& OutResult) override
    {
        return TakeDamage(InHitData, OutResult);
    }

    virtual bool TakeDamage(const FHitData& InHitData, EAttackResult& OutResult) override;
    virtual bool IsAlive() const override;
    virtual FName GetHeadSocket() const override;

    // ICanGetEffects
    virtual bool CanEffectBeApplied(EEffectType Type, AActor* Applier) override;

    // IRotatingInterface
    virtual FRotator GetDesiredRotation() const override;

    // IMontageManagerInterface
    virtual UDataTable* GetMontages(EMontageAction InAction) const override;

public:

    UFUNCTION(BlueprintCallable)
    ABaseAIController* GetAIController() const { return AIController; }

private:

    UPROPERTY(EditAnywhere, Category = "AI")
    UBehaviorTree* BTree;

    // find in beginplay
    UPROPERTY()
        ABaseAIController* AIController;

    // ???
    UPROPERTY()
    UStatBarUI* HealthUI;

    TArray<AActor*> AttachedActors;

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
    UPROPERTY(EditAnywhere, Category = "Components")
        UWidgetComponent* TargetWidget;

    // find from hierachy
    UPROPERTY(EditAnywhere, Category = "Components")
        UWidgetComponent* StatBarsWidget;


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

};
