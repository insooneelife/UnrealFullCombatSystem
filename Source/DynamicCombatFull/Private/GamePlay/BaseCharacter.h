// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCore/DataTypes.h"

#include "Interfaces/CanBeAttacked.h"
#include "Interfaces/CanGetEffects.h"
#include "Interfaces/RotatingInterface.h"
#include "Interfaces/MontageManagerInterface.h"
#include "Interfaces/IsArcher.h"
#include "Interfaces/CanOpenUI.h"
#include "Interfaces/AbilityInterface.h"
#include "Components/TimelineComponent.h"

#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAudioComponent;
class URotatingComponent;
class UEquipmentComponent;
class UInventoryComponent;
class UDissolveComponent;
class UStatsManagerComponent;
class UMovementSpeedComponent;
class UEffectsComponent;
class USceneComponent;
class UDynamicTargetingComponent;
class UMontageManagerComponent;
class UExtendedStatComponent;
class UCollisionHandlerComponent;
class UArrowComponent;
class UInputBufferComponent;
class UStateManagerComponent;
class UAbilityComponent;
class UTimelineComponent;
class UInGameUI;
class UKeybindingsUI;
class UTexture2D;
class AAbilityBase;

UCLASS()
class ABaseCharacter 
    :
    public ACharacter, 
    public ICanBeAttacked,
    public ICanGetEffects,
    public IRotatingInterface,
    public IMontageManagerInterface,
    public IIsArcher,
    public ICanOpenUI,
    public IAbilityInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

protected:
    // effects events
    void OnEffectApplied(EEffectType InType);
    void OnEffectRemoved(EEffectType InType);


    // weapon collision events
    void OnHit(const FHitResult& Hit);
    void OnCollisionActivated(ECollisionPart CollisionPart);

    // input buffer events
    void OnInputBufferConsumed(EInputBufferKey InKey);
    void OnInputBufferClose();

    // zoom events
    void UpdateZoom();

    // return true if completed
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    bool UpdateZoomTimeline(
        bool bInCondition,
        TEnumAsByte<ETimelineDirection::Type>& OutDirection,
        float& OutAlpha);

    void OnActionPressed_Zoom();
    void OnActionReleased_Zoom();

    // block events
    void UpdateBlocking();

    // return true if completed
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    bool UpdateBlockingTimeline(
        bool bInCondition,
        TEnumAsByte<ETimelineDirection::Type>& OutDirection,
        float& OutAlpha);


    void OnActionPressed_Block();
    void OnActionReleased_Block();

    // melee attacks events
    void OnActionPressed_Parry();
    void OnActionPressed_SpecialAttack();
    void OnActionPressed_ThrustAttack();
    void OnActionPressed_HeavyAttack();
    void OnActionPressed_Attack();

    // targeting system events
    void OnActionPressed_ToggleTargeting();
    void OnActionPressed_TargetLeft();
    void OnActionPressed_TargetRight();
    void OnTargetingToggled(bool bInEnabled);

    // equipment events
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex);
    void OnCombatTypeChanged(ECombatType CombatType);
    void OnMainHandTypeChanged(EItemType InType);
    void OnInCombatChanged(bool bIsInCombat);

    void OnActionPressed_SwitchMainHandTypeUp();
    void OnActionPressed_SwitchMainHandTypeDown();
    void OnActionPressed_SwitchMainHandItemUp();
    void OnActionPressed_SwitchMainHandItemDown();
    void OnActionPressed_SwitchArrowsItem();
    void OnActionPressed_SwitchToolItem();
    void OnActionPressed_UseToolItem();

    void PlayMainHandTypeChangedMontage(EItemType InType);

    // movement && camera
    void OnAxis_MoveForward(float AxisValue);
    void OnAxis_MoveRight(float AxisValue);
    void OnAxis_HorizontalLook(float AxisValue);
    void OnAxis_VerticalLook(float AxisValue);

    void OnActionPressed_ToggleMovement();
    void OnActionPressed_Sprint();
    void OnActionReleased_Sprint();

    void OnMovementStateStart(EMovementState InState);
    void OnMovementStateEnd(EMovementState InState);

    void OnActionPressed_Roll();
    void OnActionPressed_Jump();
    void OnActionReleased_Jump();

    // state machine events
    void OnStateChanged(EState PrevState, EState NewState);

    // interaction events
    void OnActionPressed_Interact();
    void CheckForInteractable();


    // activities events
    void OnActivityChanged(EActivity Activity, bool bValue);
    void StartLookingForward();
    void StopLookingForward();

    void StartAiming();
    void StopAiming();
    void StartBlocking();
    void StopBlocking();
    void StartZooming();
    void StopZooming();

    // toggle ui events
    void OnActionPressed_Inventory();
    void OnActionPressed_Equipment();

    // stats events
    void OnValueChanged_ExtendedHealth(float NewValue, float MaxValue);
    void OnValueChanged_ExtendedStamina(float NewValue, float MaxValue);

    // rotating events
    void OnRotatingEnd();

    // toggle combat events
    void OnActionPressed_ToggleCombat();

    // slow motion events
    void OnActionPressed_SlowMotion();
    void OnActionReleased_SlowMotion();

    // archery events
    void OnActionPressed_BowAttack();
    void OnActionReleased_BowAttack();

    void PlayBowDrawSound();
    void AttemptPlayBowDrawSound();
    void AttemptPlayBowDrawSoundDelayed();

    void ShootArrow();
    void StopBowDrawSound();

    // ability events
    void OnKeyPressed_1();
    void OnKeyPressed_2();
    void OnKeyPressed_3();
    void OnKeyPressed_4();
    void OnKeyPressed_5();

    void OnManaConsumed(float Amount);
    void OnValueChanged_ExtendedMana(float NewValue, float MaxValue);

    void AbilityPressed();
    void AbilityReleased();
    void UpdateSpellActiveIndexKey(int NewActiveIndex);
    void UpdateAbilityCrosshair();
    void UpdateAbilityPressed();

    void OnAbilityStarted();
    void OnAbilityEnded(EAbilityEndResult Result);

    void UpdateAbilityAttackKey();
    void EnableAbilityMode();
    void DisableAbilityMode();

    void OnMousePressed_Thumb();
    void OnMouseReleased_Thumb();

    void OnAbilityChanged(AAbilityBase* NewAbility);

    void AbilityOnStateChanged(EState NewState);
    void AbilityOnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode);
    void SetSpellActiveIndex();
    void OnActionPressed_AbilityAttack();
    void OnActionReleased_AbilityAttack();

    void OnKeyPressed_P();
    void OnKeyPressed_K();
    void OnKeyReleased_K();
    void OnKeyPressed_L();
    void OnKeyPressed_O();
    void CreateKeybindings();

public:

    FTransform GetSpawnArrowTransform() const;

    FRotator GetArrowSpawnDirection(
        FVector InCameraDirection,
        FVector InCurrentTraceDirection, 
        FVector InImpactPoint, 
        FVector InArrowSpawnLocation) const;

    FHitData MakeMeleeHitData(AActor* HitActor);
    void ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower);

    void MeleeAttack(EMeleeAttackType InType);
    void Roll();
    void CustomJump();
    void ToggleCombat();
    void Parry();
    bool AttemptBackstab();
    void UseItem(EItemType InType);

    bool CanMeleeAttack() const;
    bool CanRoll() const;
    bool CanBeAttacked() const;
    bool CanBeStunned() const;
    bool CanUseOrSwitchItem() const;
    bool CanOpenUI() const;
    bool CanBowAttack() const;
    bool CanEnterSlowMotion() const;

    UAnimMontage* GetMeleeAttackMontage(EMeleeAttackType AttackType);
    UAnimMontage* GetRollMontage() const;
    UAnimMontage* GetStunMontage(EDirection Direction) const;
    UAnimMontage* GetBlockMontage() const;
    UAnimMontage* GetImpactMontage() const;
    UAnimMontage* GetParriedMontage() const;
    UAnimMontage* GetParryMontage() const;

    void ResetMeleeAttackCounter();
    void ResetAimingMode();

    void ShowCrosshair(UTexture2D* InTexture);
    void HideCrosshair();

    void UpdateAimAlpha();

    void UpdateRotationSettings();

    void StartSlowMotion();
    void StopSlotMotion();
    void LoopSlowMotion();

    void Block();
    void Stunned();
    void Death();
    void Parried();
    void Impact();
    void Backstabbed();

    void SetCameraLagSmoothly(float InTargetLagSpeed);
    void UpdateCameraLag();
    void SetSprint(bool bActivate);

    void CalculateLeanAmount(float& OutLeanAmount, float& OutInterpSpeed);
    void SprintLoop();

public:
    // ICanBeAttacked
    virtual bool TakeDamage(const FHitData& HitData, EAttackResult& OutResult) override;
    virtual bool IsAlive() const override;
    virtual FName GetHeadSocket() const override;

    // ICanGetEffects
    virtual bool CanEffectBeApplied(EEffectType Type, AActor* Applier) override;

    // IRotatingInterface
    virtual FRotator GetDesiredRotation() const override;

    // IMontageManagerInterface
    virtual UDataTable* GetMontages(EMontageAction InAction) const override;

    // IIsArcher
    virtual float GetAimAlpha() const override;
    virtual bool DoesHoldBowString() const override;
    virtual FName GetBowStringSocketName() const override;

    // ICanOpenUI
    virtual void OpenedUI() override;
    virtual void ClosedUI() override;

    // IAbilityInterface
    virtual bool CanCastAbility() const override;
    virtual float GetMagicDamage() const override;
    virtual float GetCastingSpeed() const override;

public:
    bool IsStateEqualPure(EState InState) const;
    bool IsEnoughStamina(float InValue) const;
    void LineTraceForInteractable();
    void GetMovementVectors(FVector& InOutForward, FVector& InOutRight) const;
    bool IsCombatTypePure(ECombatType InType) const;
    bool IsActivityPure(EActivity InActivity) const;
    bool IsIdleAndNotFalling() const;
    bool HasMovementInput() const;
    bool IsCharacterAlive() const;
    void UpdateReceivedHitDirection(FVector InHitFromDirection);
    bool CanUseOrSwitch() const;
    bool CanBeInterrupted() const;


public:
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
    void SetTimerRetriggerable(FTimerHandle& TimerHandle, TBaseDelegate<void> ObjectDelegate, float Time, bool bLoop);


private:

    UPROPERTY()
        UCameraComponent* FollowCamera;

    UPROPERTY()
        USpringArmComponent* CameraBoom;

    int MeleeAttackCounter;
    EMeleeAttackType MeleeAttackType;
    float BlockAlpha;

    UPROPERTY()
    AActor* BackstabbedActor;

    EDirection ReceivedHitDirection;

    TArray<FName> LeftHandCollisionSockets;
    TArray<FName> RightHandCollisionSockets;
    TArray<FName> RightFootCollisionSockets;
    TArray<FName> LeftFootCollisionSockets;

    UPROPERTY()
    UAudioComponent* EffectsAudio;
    
    UPROPERTY()
    URotatingComponent* Rotating;

    UPROPERTY()
    UEquipmentComponent* Equipment;

    UPROPERTY()
    UInventoryComponent* Inventory;

    UPROPERTY()
    UDissolveComponent* Dissolve;

    UPROPERTY()
    UStatsManagerComponent* StatsManager;

    UPROPERTY()
    UMovementSpeedComponent* MovementSpeed;

    UPROPERTY()
    UEffectsComponent* Effects;
    
    UPROPERTY()
    USceneComponent* ArrowSpawnLocation;
    
    UPROPERTY()
    UDynamicTargetingComponent* DynamicTargeting;
    
    UPROPERTY()
    UMontageManagerComponent* MontageManager;
    
    UPROPERTY()
    UExtendedStatComponent* ExtendedStamina;
    
    UPROPERTY()
    UExtendedStatComponent* ExtendedHealth;
    
    UPROPERTY()
    UExtendedStatComponent* ExtendedMana;
    
    UPROPERTY()
    UCollisionHandlerComponent* MeleeCollisionHandler;
    
    UPROPERTY()
    UArrowComponent* TargetingArrow;
    
    UPROPERTY()
    UInputBufferComponent* InputBuffer;
    
    UPROPERTY()
    UStateManagerComponent* StateManager;
    
    UPROPERTY()
    UAbilityComponent* AbilityComponent;

    bool IsCrosshairVisible;
    float AimAlpha;
    bool bIsInSlowMotion;
    float SlowMotionTimeDilation;
    float SlowMotionStaminaCost;

    UPROPERTY()
    UInGameUI* InGameWidget;
    
    UPROPERTY()
    AActor* InteractionActor;
    
    UPROPERTY()
    UKeybindingsUI* KeybindingsWidget;

    bool bAutoZoom;
    float TargetLagSpeed;
    float ZoomAlpha;
    float ZoomCameraArmLength;
    float InitialCameraArmLength;
    float InitialCameraLagSpeed;

    bool bIsAbilityInputPressed;
    bool bIsAbilityMainInputPressed;

    int SelectedSpellIndex;

    float HorizontalLookRate;
    float VerticalLookRate;

    EMovementState StoredMovementState;
    float RollStaminaCost;
    float SprintStaminaCost;

    // WidgetBlueprint'/Game/DynamicCombatSystem/Widgets/InGameWB.InGameWB'
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> InGameUIClass;

    // WidgetBlueprint'/Game/DynamicCombatSystem/Widgets/KeybindingsWB.KeybindingsWB'
    UPROPERTY(EditAnywhere)
        TSubclassOf<UKeybindingsUI> KeybindingsUIClass;

    // Texture2D'/Game/DynamicCombatSystem/Widgets/Textures/T_AbilityCrosshair.T_AbilityCrosshair'
    UPROPERTY(EditAnywhere)
    UTexture2D* CrosshairTexture;

    // Texture2D'/Game/DynamicCombatSystem/Widgets/Textures/T_Crosshair.T_Crosshair'
    UPROPERTY(EditAnywhere)
        UTexture2D* DefaultCrosshairTextureObject;

    FTimerHandle SprintLoopTimerHandle;
    FTimerHandle StopLookingForwardTimerHandle;
    FTimerHandle HideCrosshairTimerHandle;
    FTimerHandle UpdateZoomTimerHandle;

    FTimerHandle CheckForInteractableTimerHandle;
    FTimerHandle PlayBowDrawSoundTimerHandle;

    FTimerHandle RetriggerableDelayTimerHandle;

    FTimerHandle ResetMeleeAttackCounterTimerHandle;

    FTimerHandle LoopSlowMotionTimerHandle;

    FTimerHandle UpdateCameraLagTimerHandle;

    


    UPROPERTY(EditAnywhere)
        UDataTable* PlayerMeleeMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerArcherMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerCommonMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerMagicMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerUnarmedMontages;
};
