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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

public:	
    virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

protected:
    // effects events
    UFUNCTION(BlueprintCallable)
    void OnEffectApplied(EEffectType InType);

    UFUNCTION(BlueprintCallable)
    void OnEffectRemoved(EEffectType InType);


    // weapon collision events
    UFUNCTION(BlueprintCallable)
    void OnHit(const FHitResult& Hit);

    UFUNCTION(BlueprintCallable)
    void OnCollisionActivated(ECollisionPart CollisionPart);

    // input buffer events
    UFUNCTION(BlueprintCallable)
    void OnInputBufferConsumed(EInputBufferKey InKey);

    UFUNCTION(BlueprintCallable)
    void OnInputBufferClose();

    // zoom events
    void UpdateZoom();

    // return true if completed
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    void UpdateZoomTimeline(bool bInCondition);

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Zoom();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_Zoom();


    // block events
    void UpdateBlocking();

    // return true if completed
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    void UpdateBlockingTimeline(bool bInCondition);

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Block();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_Block();

    // melee attacks events
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Parry();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SpecialAttack();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_ThrustAttack();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_HeavyAttack();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Attack();


    // targeting system events
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_ToggleTargeting();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_TargetLeft();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_TargetRight();

    UFUNCTION(BlueprintCallable)
    void OnTargetingToggled(bool bInEnabled);


    // equipment events
    UFUNCTION(BlueprintCallable)
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex);
    
    UFUNCTION(BlueprintCallable)
    void OnCombatTypeChanged(ECombatType CombatType);

    UFUNCTION(BlueprintCallable)
    void OnMainHandTypeChanged(EItemType InType);

    UFUNCTION(BlueprintCallable)
    void OnInCombatChanged(bool bIsInCombat);


    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SwitchMainHandTypeUp();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SwitchMainHandTypeDown();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SwitchMainHandItemUp();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SwitchMainHandItemDown();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SwitchArrowsItem();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SwitchToolItem();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_UseToolItem();


    void PlayMainHandTypeChangedMontage(EItemType InType);

    // movement && camera
    UFUNCTION(BlueprintCallable)
    void OnAxis_MoveForward(float AxisValue);

    UFUNCTION(BlueprintCallable)
    void OnAxis_MoveRight(float AxisValue);

    UFUNCTION(BlueprintCallable)
    void OnAxis_HorizontalLook(float AxisValue);

    UFUNCTION(BlueprintCallable)
    void OnAxis_VerticalLook(float AxisValue);


    UFUNCTION(BlueprintCallable)
    void OnActionPressed_ToggleMovement();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Sprint();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_Sprint();


    UFUNCTION(BlueprintCallable)
    void OnMovementStateStart(EMovementState InState);

    UFUNCTION(BlueprintCallable)
    void OnMovementStateEnd(EMovementState InState);


    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Roll();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Jump();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_Jump();


    // state machine events
    UFUNCTION(BlueprintCallable)
    void OnStateChanged(EState PrevState, EState NewState);

    // interaction events
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Interact();

    UFUNCTION(BlueprintCallable)
    void CheckForInteractable();


    // activities events
    UFUNCTION(BlueprintCallable)
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
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Inventory();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_Equipment();


    // stats events
    UFUNCTION(BlueprintCallable)
    void OnValueChanged_ExtendedHealth(float NewValue, float MaxValue);

    UFUNCTION(BlueprintCallable)
    void OnValueChanged_ExtendedStamina(float NewValue, float MaxValue);


    // rotating events
    UFUNCTION(BlueprintCallable)
    void OnRotatingEnd();

    // toggle combat events
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_ToggleCombat();

    // slow motion events
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_SlowMotion();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_SlowMotion();


    // archery events
    UFUNCTION(BlueprintCallable)
    void OnActionPressed_BowAttack();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_BowAttack();

    void PlayBowDrawSound();
    void AttemptPlayBowDrawSound();
    void AttemptPlayBowDrawSoundDelayed();

    void ShootArrow();
    void StopBowDrawSound();

    // ability events
    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_1();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_2();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_3();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_4();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_5();

    UFUNCTION(BlueprintCallable)
    void OnManaConsumed(float Amount);

    UFUNCTION(BlueprintCallable)
    void OnValueChanged_ExtendedMana(float NewValue, float MaxValue);


    void AbilityPressed();
    void AbilityReleased();
    void UpdateSpellActiveIndexKey(int NewActiveIndex);
    void UpdateAbilityCrosshair();
    void UpdateAbilityPressed();

    UFUNCTION(BlueprintCallable)
    void OnAbilityStarted();

    UFUNCTION(BlueprintCallable)
    void OnAbilityEnded(EAbilityEndResult Result);

    void UpdateAbilityAttackKey();
    void EnableAbilityMode();
    void DisableAbilityMode();

    UFUNCTION(BlueprintCallable)
    void OnMousePressed_Thumb();

    UFUNCTION(BlueprintCallable)
    void OnMouseReleased_Thumb();

    UFUNCTION(BlueprintCallable)
    void OnAbilityChanged(AAbilityBase* NewAbility);

    void AbilityOnStateChanged(EState NewState);
    void AbilityOnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode);
    void SetSpellActiveIndex();

    UFUNCTION(BlueprintCallable)
    void OnActionPressed_AbilityAttack();

    UFUNCTION(BlueprintCallable)
    void OnActionReleased_AbilityAttack();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_P();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_K();

    UFUNCTION(BlueprintCallable)
    void OnKeyReleased_K();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_L();

    UFUNCTION(BlueprintCallable)
    void OnKeyPressed_O();

    UFUNCTION(BlueprintCallable)
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

    UFUNCTION(BlueprintCallable)
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
    UFUNCTION(BlueprintCallable)
    virtual float GetAimAlpha() const override { return AimAlpha; }
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

    UFUNCTION(BlueprintCallable)
    EItemType GetSelectedMainHandType() const;

    UFUNCTION(BlueprintCallable)
    EState GetState() const;

private:
    void SetTimerRetriggerable(FTimerHandle& TimerHandle, TBaseDelegate<void> ObjectDelegate, float Time, bool bLoop);

    void SetData();

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

    UPROPERTY()
    UAudioComponent* EffectsAudio;
    
    UPROPERTY()
    URotatingComponent* Rotating;

    UPROPERTY(EditAnywhere)
    UEquipmentComponent* Equipment;

    UPROPERTY(EditAnywhere)
    UInventoryComponent* Inventory;

    UPROPERTY(EditAnywhere)
    UDissolveComponent* Dissolve;

    UPROPERTY(EditAnywhere)
    UStatsManagerComponent* StatsManager;

    UPROPERTY(EditAnywhere)
    UMovementSpeedComponent* MovementSpeed;

    UPROPERTY(EditAnywhere)
    UEffectsComponent* Effects;
    
    UPROPERTY()
    USceneComponent* ArrowSpawnLocation;
    
    UPROPERTY(EditAnywhere)
    UDynamicTargetingComponent* DynamicTargeting;
    
    UPROPERTY()
    UMontageManagerComponent* MontageManager;
    
    UPROPERTY(EditAnywhere)
    UExtendedStatComponent* ExtendedStamina;
    
    UPROPERTY(EditAnywhere)
    UExtendedStatComponent* ExtendedHealth;
    
    UPROPERTY(EditAnywhere)
    UExtendedStatComponent* ExtendedMana;
    
    UPROPERTY(EditAnywhere)
    UCollisionHandlerComponent* MeleeCollisionHandler;
    
    UPROPERTY()
    UArrowComponent* TargetingArrow;
    
    UPROPERTY()
    UInputBufferComponent* InputBuffer;
    
    UPROPERTY()
    UStateManagerComponent* StateManager;
    
    UPROPERTY(EditAnywhere)
    UAbilityComponent* AbilityComponent;

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

    UPROPERTY()
    UInGameUI* InGameWidget;
    
    UPROPERTY()
    AActor* InteractionActor;
    
    UPROPERTY()
    UKeybindingsUI* KeybindingsWidget;

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

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> InGameUIClass;

    UPROPERTY(EditAnywhere)
        TSubclassOf<UKeybindingsUI> KeybindingsUIClass;

    UPROPERTY(EditAnywhere)
    UTexture2D* CrosshairTexture;

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
        UDataTable* PlayerOneHandMeleeMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerArcherMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerCommonMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerMagicMontages;

    UPROPERTY(EditAnywhere)
        UDataTable* PlayerUnarmedMontages;
};
