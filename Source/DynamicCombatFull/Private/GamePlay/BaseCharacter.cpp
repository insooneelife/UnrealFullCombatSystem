// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    ReceivedHitDirection = EDirection::Front;

    LeftHandCollisionSockets = 
    {
        FName(TEXT("left_hand_1")),
        FName(TEXT("left_hand_2")),
    };

    RightHandCollisionSockets =
    {
        FName(TEXT("right_hand_1")),
        FName(TEXT("right_hand_2")),
    };

    RightFootCollisionSockets =
    {
        FName(TEXT("right_foot_1")),
        FName(TEXT("right_foot_2")),
    };

    LeftFootCollisionSockets =
    {
        FName(TEXT("left_foot_1")),
        FName(TEXT("left_foot_2")),
    };

    SlowMotionTimeDilation = 0.4f;
    SlowMotionStaminaCost = 1.0f;
    ZoomCameraArmLength = 150.0f;

    HorizontalLookRate = 45.0f;
    VerticalLookRate = 45.0f;
    StoredMovementState = EMovementState::Idle;
    RollStaminaCost = 25.0f;
    SprintStaminaCost = 0.5f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    //MovementModeChangedDelegate.AddDynamic
}

void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindKey(EKeys::O, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_O);
    //PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);
    //PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);
}

void ABaseCharacter::OnEffectApplied(EEffectType InType)
{
}

void ABaseCharacter::OnEffectRemoved(EEffectType InType)
{
}

void ABaseCharacter::OnHit(const FHitResult& Hit)
{
}

void ABaseCharacter::OnCollisionActivated(ECollisionPart CollisionPart)
{
}

void ABaseCharacter::OnInputBufferConsumed(EInputBufferKey InKey)
{
}

void ABaseCharacter::OnInputBufferClose()
{
}

void ABaseCharacter::UpdateZoom()
{
}

void ABaseCharacter::OnActionPressed_Zoom()
{
}

void ABaseCharacter::OnActionReleased_Zoom()
{
}

void ABaseCharacter::UpdateBlocking()
{
}

void ABaseCharacter::OnActionPressed_Block()
{
}

void ABaseCharacter::OnActionReleased_Block()
{
}

void ABaseCharacter::OnActionPressed_Parry()
{
}

void ABaseCharacter::OnActionPressed_SpecialAttack()
{
}

void ABaseCharacter::OnActionPressed_ThrustAttack()
{
}

void ABaseCharacter::OnActionPressed_HeavyAttack()
{
}

void ABaseCharacter::OnActionPressed_Attack()
{
}

void ABaseCharacter::OnActionPressed_ToggleTargeting()
{
}

void ABaseCharacter::OnActionPressed_TargetLeft()
{
}

void ABaseCharacter::OnActionPressed_TargetRight()
{
}

void ABaseCharacter::OnTargetingToggled(bool bInEnabled)
{
}

void ABaseCharacter::OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex)
{
}

void ABaseCharacter::OnCombatTypeChanged(ECombatType CombatType)
{
}

void ABaseCharacter::OnMainHandTypeChanged(EItemType InType)
{
}

void ABaseCharacter::OnInCombatChanged(bool bIsInCombat)
{
}

void ABaseCharacter::OnActionPressed_SwitchMainHandTypeUp()
{
}

void ABaseCharacter::OnActionPressed_SwitchMainHandTypeDown()
{
}

void ABaseCharacter::OnActionPressed_SwitchMainHandItemUp()
{
}

void ABaseCharacter::OnActionPressed_SwitchMainHandItemDown()
{
}

void ABaseCharacter::OnActionPressed_SwitchArrowsItem()
{
}

void ABaseCharacter::OnActionPressed_SwitchToolItem()
{
}

void ABaseCharacter::OnActionPressed_UseToolItem()
{
}

void ABaseCharacter::PlayMainHandTypeChangedMontage(EItemType InType)
{
}

void ABaseCharacter::OnAxis_MoveForward(float AxisValue)
{
}

void ABaseCharacter::OnAxis_MoveRight(float AxisValue)
{
}

void ABaseCharacter::OnAxis_HorizontalLook(float AxisValue)
{
}

void ABaseCharacter::OnAxis_VerticalLook(float AxisValue)
{
}

void ABaseCharacter::OnActionPressed_ToggleMovement()
{
}

void ABaseCharacter::OnActionPressed_Sprint()
{
}

void ABaseCharacter::OnActionReleased_Sprint()
{
}

void ABaseCharacter::OnMovementStateStart(EMovementState InState)
{
}

void ABaseCharacter::OnMovementStateEnd(EMovementState InState)
{
}

void ABaseCharacter::OnActionPressed_Roll()
{
}

void ABaseCharacter::OnActionPressed_Jump()
{
}

void ABaseCharacter::OnActionReleased_Jump()
{
}

void ABaseCharacter::OnMovementModeChanged(ACharacter* InCharacter, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
}

void ABaseCharacter::OnStateChanged(EState PrevState, EState NewState)
{
}

void ABaseCharacter::OnActionPressed_Interact()
{
}

void ABaseCharacter::CheckForInteractable()
{
}

void ABaseCharacter::OnActivityChanged(EActivity Activity, bool bValue)
{
}

void ABaseCharacter::StartLookingForward()
{
}

void ABaseCharacter::StopLookingForward()
{
}

void ABaseCharacter::StartAiming()
{
}

void ABaseCharacter::StopAiming()
{
}

void ABaseCharacter::StartBlocking()
{
}

void ABaseCharacter::StopBlocking()
{
}

void ABaseCharacter::StartZooming()
{
}

void ABaseCharacter::StopZooming()
{
}

void ABaseCharacter::OnActionPressed_Inventory()
{
}

void ABaseCharacter::OnActionPressed_Equipment()
{
}

void ABaseCharacter::OnValueChanged_ExtendedHealth(float NewValue, float MaxValue)
{
}

void ABaseCharacter::OnValueChanged_ExtendedStamina(float NewValue, float MaxValue)
{
}

void ABaseCharacter::OnRotatingEnd()
{
}

void ABaseCharacter::OnActionPressed_ToggleCombat()
{
}

void ABaseCharacter::OnActionPressed_SlowMotion()
{
}

void ABaseCharacter::OnActionReleased_SlowMotion()
{
}

void ABaseCharacter::OnActionPressed_BowAttack()
{
}

void ABaseCharacter::OnActionReleased_BowAttack()
{
}

void ABaseCharacter::PlayBowDrawSound()
{
}

void ABaseCharacter::AttemptPlayBowDrawSound()
{
}

void ABaseCharacter::ShootArrow()
{
}

void ABaseCharacter::StopBowDrawSound()
{
}

void ABaseCharacter::OnKeyPressed_1()
{
}

void ABaseCharacter::OnKeyPressed_2()
{
}

void ABaseCharacter::OnKeyPressed_3()
{
}

void ABaseCharacter::OnKeyPressed_4()
{
}

void ABaseCharacter::OnKeyPressed_5()
{
}

void ABaseCharacter::OnManaConsumed(float Amount)
{
}

void ABaseCharacter::OnValueChanged_ExtendedMana(float NewValue, float MaxValue)
{
}

void ABaseCharacter::AbilityPressed()
{
}

void ABaseCharacter::AbilityReleased()
{
}

void ABaseCharacter::UpdateSpellActiveIndexKey(int NewActiveIndex)
{
}

void ABaseCharacter::UpdateAbilityCrosshair()
{
}

void ABaseCharacter::UpdateAbilityPressed()
{
}

void ABaseCharacter::OnAbilityStarted()
{
}

void ABaseCharacter::OnAbilityEnded(EAbilityEndResult Result)
{
}

void ABaseCharacter::UpdateAbilityAttackKey()
{
}

void ABaseCharacter::EnableAbilityMode()
{
}

void ABaseCharacter::DisableAbilityMode()
{
}

void ABaseCharacter::OnMousePressed_Thumb()
{
}

void ABaseCharacter::OnMouseReleased_Thumb()
{
}

void ABaseCharacter::OnAbilityChanged(AAbilityBase* NewAbility)
{
}

void ABaseCharacter::AbilityOnStateChanged(EState NewState)
{
}

void ABaseCharacter::AbilityOnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode)
{
}

void ABaseCharacter::SetSpellActiveIndex()
{
}

void ABaseCharacter::OnActionPressed_AbilityAttack()
{
}

void ABaseCharacter::OnActionReleased_AbilityAttack()
{
}

void ABaseCharacter::OnKeyPressed_P()
{
}

void ABaseCharacter::OnKeyPressed_K()
{
}

void ABaseCharacter::OnKeyPressed_L()
{
}

void ABaseCharacter::OnKeyPressed_O()
{
}

void ABaseCharacter::CreateKeybindings()
{
}

FTransform ABaseCharacter::GetSpawnedArrowTransform() const
{
    return FTransform();
}

FHitData ABaseCharacter::MakeMeleeHitData(AActor* HitActor)
{
    return FHitData();
}

void ABaseCharacter::ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower)
{
}

void ABaseCharacter::MeleeAttack(EMeleeAttackType InType)
{
}

void ABaseCharacter::Roll()
{
}

void ABaseCharacter::CustomJump()
{
}

void ABaseCharacter::ToggleCombat()
{
}

void ABaseCharacter::Parry()
{
}

bool ABaseCharacter::AttemptBackstab()
{
    return false;
}

void ABaseCharacter::UseItem(EItemType InType)
{
}

bool ABaseCharacter::CanMeleeAttack() const
{
    return false;
}

bool ABaseCharacter::CanRoll() const
{
    return false;
}

bool ABaseCharacter::CanBeAttacked() const
{
    return false;
}

bool ABaseCharacter::CanBeStunned() const
{
    return false;
}

bool ABaseCharacter::CanUseOrSwitchItem() const
{
    return false;
}

bool ABaseCharacter::CanOpenUI() const
{
    return false;
}

bool ABaseCharacter::CanBowAttack() const
{
    return false;
}

bool ABaseCharacter::CanEnterSlowMotion() const
{
    return false;
}

UAnimMontage* ABaseCharacter::GetMeleeAttackMontage(EMeleeAttackType AttackType) const
{
    return nullptr;
}

UAnimMontage* ABaseCharacter::GetRollMontage() const
{
    return nullptr;
}

UAnimMontage* ABaseCharacter::GetStunMontage(EDirection Direction) const
{
    return nullptr;
}

UAnimMontage* ABaseCharacter::GetBlockMontage() const
{
    return nullptr;
}

UAnimMontage* ABaseCharacter::GetImpactMontage() const
{
    return nullptr;
}

UAnimMontage* ABaseCharacter::GetParriedMontage() const
{
    return nullptr;
}

UAnimMontage* ABaseCharacter::GetParryMontage() const
{
    return nullptr;
}

void ABaseCharacter::ResetMeleeAttackCounter()
{
}

void ABaseCharacter::ResetAimingMode()
{
}

void ABaseCharacter::ShowCrosshair(UTexture2D* InTexture)
{
}

void ABaseCharacter::HideCrosshair()
{
}

void ABaseCharacter::UpdateAimAlpha()
{
}

void ABaseCharacter::UpdateRotationSettings()
{
}

void ABaseCharacter::StartSlowMotion()
{
}

void ABaseCharacter::StopSlotMotion()
{
}

void ABaseCharacter::LoopSlowMotion()
{
}

void ABaseCharacter::Block()
{
}

void ABaseCharacter::Stunned()
{
}

void ABaseCharacter::Death()
{
}

void ABaseCharacter::Parried()
{
}

void ABaseCharacter::Impact()
{
}

void ABaseCharacter::Backstabbed()
{
}

void ABaseCharacter::SetCameraLagSmoothly(float InTargetLagSpeed)
{
}

void ABaseCharacter::UpdateCameraLag()
{
}

void ABaseCharacter::SetSprint(bool bActivate)
{
}

void ABaseCharacter::CalculateLeanAmount(float& OutLeanAmount, float& OutInterpSpeed)
{
}

void ABaseCharacter::SprintLoop()
{
}

bool ABaseCharacter::TakeDamage(const FHitData& HitData, EAttackResult& OutResult)
{
    return false;
}

bool ABaseCharacter::IsAlive() const
{
    return false;
}

FName ABaseCharacter::GetHeadSocket() const
{
    return FName();
}

bool ABaseCharacter::CanEffectBeApplied(EEffectType Type, AActor* Applier)
{
    return false;
}

FRotator ABaseCharacter::GetDesiredRotation() const
{
    return FRotator();
}

UDataTable* ABaseCharacter::GetMontages() const
{
    return nullptr;
}

float ABaseCharacter::GetAimAlpha() const
{
    return 0.0f;
}

bool ABaseCharacter::DoesHoldBowString() const
{
    return false;
}

FName ABaseCharacter::GetBowStringSocketName() const
{
    return FName();
}

void ABaseCharacter::OpenedUI()
{
}

void ABaseCharacter::ClosedUI()
{
}

bool ABaseCharacter::CanCastAbility() const
{
    return false;
}

float ABaseCharacter::GetMagicDamage() const
{
    return 0.0f;
}

float ABaseCharacter::GetCastingSpeed() const
{
    return 0.0f;
}

