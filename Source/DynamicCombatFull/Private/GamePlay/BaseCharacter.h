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
#include "Interfaces/IsTargetable.h"
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
class UWidgetComponent;

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
    public IAbilityInterface,
    public IIsTargetable

{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

public:	
    virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

public:
    UFUNCTION(BlueprintCallable)
    EItemType GetSelectedMainHandType() const;

    UFUNCTION(BlueprintCallable)
    EState GetState() const;

    UFUNCTION(BlueprintCallable)
    void CalculateLeanAmount(float& OutLeanAmount, float& OutInterpSpeed) const;

    UFUNCTION(BlueprintCallable)
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintCallable)
    USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    UFUNCTION(BlueprintCallable)
    float GetZoomAlpha() const { return ZoomAlpha; }

    UFUNCTION(BlueprintCallable)
    void SetZoomAlpha(float InZoomAlpha) { ZoomAlpha = InZoomAlpha; }

    UFUNCTION(BlueprintCallable)
    float GetInitialCameraArmLength() const { return InitialCameraArmLength; }

    UFUNCTION(BlueprintCallable)
    float GetZoomCameraArmLength() const { return ZoomCameraArmLength; }

    UFUNCTION(BlueprintCallable)
    UMovementSpeedComponent* GetMovementSpeedComp() const { return MovementSpeed; }

    UFUNCTION(BlueprintCallable)
    UExtendedStatComponent* GetExtendedStaminaComp() const { return ExtendedStamina; }

    UFUNCTION(BlueprintCallable)
    bool IsInSlowMotion() const { return bIsInSlowMotion; }

    UFUNCTION(BlueprintCallable)
    float GetBlockAlpha() const { return BlockAlpha; }

    UFUNCTION(BlueprintCallable)
    void SetBlockAlpha(float InBlockAlpha) { BlockAlpha = InBlockAlpha; }

public:
    // interfaces
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

    // IIsArcher
    UFUNCTION(BlueprintCallable)
    virtual float GetAimAlpha() const override { return AimAlpha; }
    virtual bool DoesHoldBowString() const override;
    virtual FName GetBowStringSocketName() const override;

    // ICanOpenUI
    virtual void OpenedUI() override;
    virtual void ClosedUI() override;

    // IAbilityInterface
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "CanCastAbility", ScriptName = "CanCastAbility"))
    bool K2_CanCastAbility() const;

    virtual bool K2_CanCastAbility_Implementation() const override { return CanCastAbility(); }

    virtual bool CanCastAbility() const override;
    virtual float GetMagicDamage() const override;
    virtual float GetCastingSpeed() const override;

    // IIsTargetable
    virtual void OnSelected() override;
    virtual void OnDeselected() override;
    virtual bool IsTargetable() const override;

protected:
    // effects events
    UFUNCTION()
    void OnEffectApplied(EEffectType InType);

    UFUNCTION()
    void OnEffectRemoved(EEffectType InType);


    // weapon collision events
    UFUNCTION()
    void OnHit(const FHitResult& InHit);

    UFUNCTION()
    void OnCollisionActivated(ECollisionPart CollisionPart);

    // input buffer events
    UFUNCTION()
    void OnInputBufferConsumed(EInputBufferKey InKey);

    UFUNCTION()
    void OnInputBufferClose();

    UFUNCTION()
    void OnActionPressed_Zoom();

    UFUNCTION()
    void OnActionReleased_Zoom();

    UFUNCTION()
    void OnActionPressed_Block();

    UFUNCTION()
    void OnActionReleased_Block();

    // melee attacks events
    UFUNCTION()
    void OnActionPressed_Parry();

    UFUNCTION()
    void OnActionPressed_SpecialAttack();

    UFUNCTION()
    void OnActionPressed_ThrustAttack();

    UFUNCTION()
    void OnActionPressed_HeavyAttack();

    UFUNCTION()
    void OnActionPressed_Attack();


    // targeting system events
    UFUNCTION()
    void OnActionPressed_ToggleTargeting();

    UFUNCTION()
    void OnActionPressed_TargetLeft();

    UFUNCTION()
    void OnActionPressed_TargetRight();

    UFUNCTION()
    void OnTargetingToggled(bool bInEnabled);


    // equipment events
    UFUNCTION()
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex);
    
    UFUNCTION()
    void OnCombatTypeChanged(ECombatType CombatType);

    UFUNCTION()
    void OnMainHandTypeChanged(EItemType InType);

    UFUNCTION()
    void OnInCombatChanged(bool bIsInCombat);

    // key input
    UFUNCTION()
    void OnActionPressed_SwitchMainHandTypeUp();

    UFUNCTION()
    void OnActionPressed_SwitchMainHandTypeDown();

    UFUNCTION()
    void OnActionPressed_SwitchMainHandItemUp();

    UFUNCTION()
    void OnActionPressed_SwitchMainHandItemDown();

    UFUNCTION()
    void OnActionPressed_SwitchArrowsItem();

    UFUNCTION()
    void OnActionPressed_SwitchToolItem();

    UFUNCTION()
    void OnActionPressed_UseToolItem();

    // movement && camera
    UFUNCTION()
    void OnAxis_MoveForward(float AxisValue);

    UFUNCTION()
    void OnAxis_MoveRight(float AxisValue);

    UFUNCTION()
    void OnAxis_HorizontalLook(float AxisValue);

    UFUNCTION()
    void OnAxis_VerticalLook(float AxisValue);


    UFUNCTION()
    void OnActionPressed_ToggleMovement();

    UFUNCTION()
    void OnActionPressed_Sprint();

    UFUNCTION()
    void OnActionReleased_Sprint();


    UFUNCTION()
    void OnMovementStateStart(EMovementState InState);

    UFUNCTION()
    void OnMovementStateEnd(EMovementState InState);


    UFUNCTION()
    void OnActionPressed_Roll();

    UFUNCTION()
    void OnActionPressed_Jump();

    UFUNCTION()
    void OnActionReleased_Jump();


    // state machine events
    UFUNCTION()
    void OnStateChanged(EState PrevState, EState NewState);

    // interaction events
    UFUNCTION()
    void OnActionPressed_Interact();

    UFUNCTION()
    void CheckForInteractable();


    // activities events
    UFUNCTION()
    void OnActivityChanged(EActivity Activity, bool bValue);

    // toggle ui events
    UFUNCTION()
    void OnActionPressed_Inventory();

    UFUNCTION()
    void OnActionPressed_Equipment();


    // stats events
    UFUNCTION()
    void OnValueChanged_ExtendedHealth(float InNewValue, float InMaxValue);

    UFUNCTION()
    void OnValueChanged_ExtendedStamina(float InNewValue, float InMaxValue);


    // rotating events
    UFUNCTION()
    void OnRotatingEnd();

    // toggle combat events
    UFUNCTION()
    void OnActionPressed_ToggleCombat();

    // slow motion events
    UFUNCTION()
    void OnActionPressed_SlowMotion();

    UFUNCTION()
    void OnActionReleased_SlowMotion();


    // archery events
    UFUNCTION()
    void OnActionPressed_BowAttack();

    UFUNCTION()
    void OnActionReleased_BowAttack();

    // ability events
    UFUNCTION()
    void OnKeyPressed_1();

    UFUNCTION()
    void OnKeyPressed_2();

    UFUNCTION()
    void OnKeyPressed_3();

    UFUNCTION()
    void OnKeyPressed_4();

    UFUNCTION()
    void OnKeyPressed_5();

    UFUNCTION()
    void OnManaConsumed(float Amount);

    UFUNCTION()
    void OnValueChanged_ExtendedMana(float NewValue, float MaxValue);

    UFUNCTION()
    void OnAbilityStarted();

    UFUNCTION()
    void OnAbilityEnded(EAbilityEndResult Result);


    UFUNCTION()
    void OnMousePressed_Thumb();

    UFUNCTION()
    void OnMouseReleased_Thumb();

    UFUNCTION()
    void OnAbilityChanged(AAbilityBase* NewAbility);


    UFUNCTION()
    void OnActionPressed_AbilityAttack();

    UFUNCTION()
    void OnActionReleased_AbilityAttack();

    UFUNCTION()
    void OnKeyPressed_P();

    UFUNCTION()
    void OnKeyPressed_K();

    UFUNCTION()
    void OnKeyReleased_K();

    UFUNCTION()
    void OnKeyPressed_L();

    UFUNCTION()
    void OnKeyPressed_O();

    // return true if completed
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    void UpdateZoomTimeline(bool bInCondition);

    // return true if completed
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    void UpdateBlockingTimeline(bool bInCondition);

private:
    // zoom events
    void UpdateZoom();

    // block events
    void UpdateBlocking();

    void PlayMainHandTypeChangedMontage(EItemType InType);

    void StartLookingForward();
    void StopLookingForward();
    void StartAiming();
    void StopAiming();
    void StartBlocking();
    void StopBlocking();
    void StartZooming();
    void StopZooming();

    void PlayBowDrawSound();
    void AttemptPlayBowDrawSound();
    void AttemptPlayBowDrawSoundDelayed();
    void ShootArrow();
    void StopBowDrawSound();

    void AbilityPressed();
    void AbilityReleased();
    void UpdateSpellActiveIndexKey(int NewActiveIndex);
    void UpdateAbilityCrosshair();
    void UpdateAbilityPressed();

    void UpdateAbilityAttackKey();
    void EnableAbilityMode();
    void DisableAbilityMode();

    void AbilityOnStateChanged(EState NewState);
    void AbilityOnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode);
    void SetSpellActiveIndex();

    void CreateKeybindings();

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

    UAnimMontage* GetNextMeleeAttackMontage(EMeleeAttackType AttackType);
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
    void SprintLoop();

    bool IsStateEqual(EState InState) const;
    bool IsEnoughStamina(float InValue) const;
    void GetMovementVectors(FVector& InOutForward, FVector& InOutRight) const;
    bool IsCombatTypeEqual(ECombatType InType) const;
    bool IsActivityEqual(EActivity InActivity) const;
    bool IsIdleAndNotFalling() const;
    bool HasMovementInput() const;
    bool IsCharacterAlive() const;
    bool CanUseOrSwitch() const;
    bool CanBeInterrupted() const;
    void UpdateReceivedHitDirection(FVector InHitFromDirection);
    void LineTraceForInteractable();
    void SetTimerRetriggerable(
        FTimerHandle& InTimerHandle, TBaseDelegate<void> InObjectDelegate, float InTime, bool bInLoop);
    void SetData();

private:

    UPROPERTY(EditAnywhere, Category = "Components")
    UStateManagerComponent* StateManager;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStatsManagerComponent* StatsManager;

    UPROPERTY(EditAnywhere, Category = "Components")
    UInputBufferComponent* InputBuffer;

    UPROPERTY(EditAnywhere, Category = "Components")
    UCollisionHandlerComponent* MeleeCollisionHandler;

    UPROPERTY(EditAnywhere, Category = "Components")
    UMontageManagerComponent* MontageManager;

    UPROPERTY(EditAnywhere, Category = "Components")
    UExtendedStatComponent* ExtendedHealth;

    UPROPERTY(EditAnywhere, Category = "Components")
    UExtendedStatComponent* ExtendedStamina;

    UPROPERTY(EditAnywhere, Category = "Components")
    UExtendedStatComponent* ExtendedMana;

    UPROPERTY(EditAnywhere, Category = "Components")
    UDynamicTargetingComponent* DynamicTargeting;

    UPROPERTY(EditAnywhere, Category = "Components")
    UEffectsComponent* Effects;

    UPROPERTY(EditAnywhere, Category = "Components")
    UMovementSpeedComponent* MovementSpeed;

    UPROPERTY(EditAnywhere, Category = "Components")
    URotatingComponent* Rotating;

    UPROPERTY(EditAnywhere, Category = "Components")
    UDissolveComponent* Dissolve;

    UPROPERTY(EditAnywhere, Category = "Components")
    UEquipmentComponent* Equipment;

    UPROPERTY(EditAnywhere, Category = "Components")
    UInventoryComponent* Inventory;

    UPROPERTY(EditAnywhere, Category = "Components")
    UAbilityComponent* AbilityComponent;


    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    USpringArmComponent* CameraBoom;

    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    UAudioComponent* EffectsAudio;

    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    USceneComponent* ArrowSpawnLocation;

    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    UArrowComponent* TargetingArrow;

    // find from hierachy
    UPROPERTY(EditAnywhere, Category = "Primitive Components")
    UWidgetComponent* TargetWidget;

    UPROPERTY()
    UInGameUI* InGameWidget;

    UPROPERTY()
    UKeybindingsUI* KeybindingsWidget;

    UPROPERTY()
    AActor* BackstabbedActor;

    UPROPERTY()
    AActor* InteractionActor;


    int MeleeAttackCounter;
    EMeleeAttackType MeleeAttackType;
    float BlockAlpha;

    UPROPERTY(EditAnywhere, Category = "Combat")
    EDirection ReceivedHitDirection;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> LeftHandCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> RightHandCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> RightFootCollisionSockets;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> LeftFootCollisionSockets;
    

    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<UUserWidget> InGameUIClass;

    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<UKeybindingsUI> KeybindingsUIClass;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UTexture2D* CrosshairTexture;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UTexture2D* DefaultCrosshairTextureObject;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UDataTable* PlayerOneHandMeleeMontages;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UDataTable* PlayerArcherMontages;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UDataTable* PlayerCommonMontages;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UDataTable* PlayerMagicMontages;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UDataTable* PlayerUnarmedMontages;

    UPROPERTY(EditAnywhere, Category = "Crosshair")
    bool IsCrosshairVisible;

    UPROPERTY(EditAnywhere, Category = "AimAlpha")
    float AimAlpha;

    UPROPERTY(EditAnywhere, Category = "SlowMotion")
    bool bIsInSlowMotion;

    UPROPERTY(EditAnywhere, Category = "SlowMotion")
    float SlowMotionTimeDilation;

    UPROPERTY(EditAnywhere, Category = "SlowMotion")
    float SlowMotionStaminaCost;

    UPROPERTY(EditAnywhere, Category = "Camera")
    bool bAutoZoom;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float TargetLagSpeed;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float ZoomAlpha;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float ZoomCameraArmLength;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float InitialCameraArmLength;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float InitialCameraLagSpeed;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    bool bIsAbilityInputPressed;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    bool bIsAbilityMainInputPressed;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    int SelectedSpellIndex;

    float HorizontalLookRate;
    float VerticalLookRate;

    EMovementState StoredMovementState;
    float RollStaminaCost;
    float SprintStaminaCost;

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

};
