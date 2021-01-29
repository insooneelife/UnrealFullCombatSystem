// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/RotatingComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/DissolveComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/MovementSpeedComponent.h"
#include "Components/EffectsComponent.h"
#include "Components/DynamicTargetingComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/InputBufferComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/AbilityComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Interfaces/IsInteractable.h"
#include "Interfaces/ItemHasUseMontage.h"
#include "UI/InGameUI.h"
#include "UI/KeybindingsUI.h"
#include "UI/InteractionMessageUI.h"
#include "UI/InventoryUI.h"
#include "UI/EquipmentUI.h"
#include "UI/LockIconUI.h"
#include "UI/StatBarUI.h"
#include "GamePlay/Abilities/AbilityBase.h"
#include "GameCore/DefaultGameInstance.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "GamePlay/Items/ObjectItems/ArrowItem.h"
#include "GamePlay/Projectiles/ArrowProjectileBase.h"
#include "GameCore/DCSGameMode.h"
#include "GameCore/GameUtils.h"
#include "TestActor.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    AutoPossessPlayer = EAutoReceiveInput::Player0;

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
    ArrowInitialSpeed = 7000.0f;

    
    PlayerOneHandMeleeMontages =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/PlayerOneHandMeleeMontages");

    PlayerArcherMontages =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/PlayerArcherMontages");

    PlayerCommonMontages =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/PlayerCommonMontages");

    PlayerMagicMontages =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/PlayerMagicMontages");

    PlayerUnarmedMontages =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/PlayerUnarmedMontages");

    StateManager = CreateDefaultSubobject<UStateManagerComponent>("StateManager");
    StatsManager = CreateDefaultSubobject<UStatsManagerComponent>("StatsManager");
    InputBuffer = CreateDefaultSubobject<UInputBufferComponent>("InputBuffer");
    MeleeCollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>("MeleeCollisionHandler");
    MontageManager = CreateDefaultSubobject<UMontageManagerComponent>("MontageManager");

    DynamicTargeting = CreateDefaultSubobject<UDynamicTargetingComponent>("DynamicTargeting");
    Effects = CreateDefaultSubobject<UEffectsComponent>("Effects");
    MovementSpeed = CreateDefaultSubobject<UMovementSpeedComponent>("MovementSpeed");
    Rotating = CreateDefaultSubobject<URotatingComponent>("Rotating");
    Dissolve = CreateDefaultSubobject<UDissolveComponent>("Dissolve");

    ExtendedHealth = CreateDefaultSubobject<UExtendedStatComponent>("ExtendedHealth");
    ExtendedStamina = CreateDefaultSubobject<UExtendedStatComponent>("ExtendedStamina");
    ExtendedMana = CreateDefaultSubobject<UExtendedStatComponent>("ExtendedMana");

    Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
    Equipment = CreateDefaultSubobject<UEquipmentComponent>("Equipment");
    AbilityComponent = CreateDefaultSubobject<UAbilityComponent>("AbilityComponent");
    
    TargetWidget = CreateDefaultSubobject<UWidgetComponent>("TargetWidget");
    TargetWidget->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    TargetingArrow = CreateDefaultSubobject<UArrowComponent>("TargetingArrow");
    TargetingArrow->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>("ArrowSpawnLocation");
    ArrowSpawnLocation->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

    EffectsAudio = CreateDefaultSubobject<UAudioComponent>("EffectsAudio");
    EffectsAudio->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);

    SetData();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    DynamicTargeting->Init(TargetingArrow);
    StatsManager->Init();
    Equipment->Init();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    

    InGameWidget = Cast<UInGameUI>(
        CreateWidget(PlayerController, GameUtils::GetDefaultGameInstance(GetWorld())->InGameUIClass));
    InGameWidget->AddToViewport();

    InGameWidget->GetHealthBar()->Init(ExtendedHealth);
    InGameWidget->GetManaBar()->Init(ExtendedMana);
    InGameWidget->GetStaminaBar()->Init(ExtendedStamina);

    CreateKeybindings();

    GetWorld()->GetTimerManager().SetTimer(
        CheckForInteractableTimerHandle, this, &ABaseCharacter::CheckForInteractable, 0.1f, true);

    Effects->OnEffectApplied.AddDynamic(this, &ABaseCharacter::OnEffectApplied);
    Effects->OnEffectRemoved.AddDynamic(this, &ABaseCharacter::OnEffectRemoved);

    MeleeCollisionHandler->OnHit.AddDynamic(this, &ABaseCharacter::OnHit);
    MeleeCollisionHandler->OnCollisionActivated.AddDynamic(this, &ABaseCharacter::OnCollisionActivated);

    InputBuffer->OnInputBufferConsumed.AddDynamic(this, &ABaseCharacter::OnInputBufferConsumed);
    InputBuffer->OnInputBufferClose.AddDynamic(this, &ABaseCharacter::OnInputBufferClose);

    DynamicTargeting->OnTargetingToggled.AddDynamic(this, &ABaseCharacter::OnTargetingToggled);

    Equipment->OnActiveItemChanged.AddDynamic(this, &ABaseCharacter::OnActiveItemChanged);
    Equipment->OnCombatTypeChanged.AddDynamic(this, &ABaseCharacter::OnCombatTypeChanged);
    Equipment->OnMainHandTypeChanged.AddDynamic(this, &ABaseCharacter::OnMainHandTypeChanged);
    Equipment->OnInCombatChanged.AddDynamic(this, &ABaseCharacter::OnInCombatChanged);

    MovementSpeed->OnMovementStateStart.AddDynamic(this, &ABaseCharacter::OnMovementStateStart);
    MovementSpeed->OnMovementStateEnd.AddDynamic(this, &ABaseCharacter::OnMovementStateEnd);

    StateManager->OnStateChanged.AddDynamic(this, &ABaseCharacter::OnStateChanged);
    StateManager->OnActivityChanged.AddDynamic(this, &ABaseCharacter::OnActivityChanged);

    ExtendedHealth->OnValueChanged.AddDynamic(this, &ABaseCharacter::OnValueChanged_ExtendedHealth);
    ExtendedStamina->OnValueChanged.AddDynamic(this, &ABaseCharacter::OnValueChanged_ExtendedStamina);

    Rotating->OnRotatingEnd.AddDynamic(this, &ABaseCharacter::OnRotatingEnd);

    AbilityComponent->OnManaConsumed.AddDynamic(this, &ABaseCharacter::OnManaConsumed);
    AbilityComponent->OnAbilityStarted.AddDynamic(this, &ABaseCharacter::OnAbilityStarted);
    AbilityComponent->OnAbilityEnded.AddDynamic(this, &ABaseCharacter::OnAbilityEnded);
    AbilityComponent->OnAbilityChanged.AddDynamic(this, &ABaseCharacter::OnAbilityChanged);
    ExtendedMana->OnValueChanged.AddDynamic(this, &ABaseCharacter::OnValueChanged_ExtendedMana);

}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    Effects->OnEffectApplied.RemoveDynamic(this, &ABaseCharacter::OnEffectApplied);
    Effects->OnEffectRemoved.RemoveDynamic(this, &ABaseCharacter::OnEffectRemoved);

    MeleeCollisionHandler->OnHit.RemoveDynamic(this, &ABaseCharacter::OnHit);
    MeleeCollisionHandler->OnCollisionActivated.RemoveDynamic(this, &ABaseCharacter::OnCollisionActivated);

    InputBuffer->OnInputBufferConsumed.RemoveDynamic(this, &ABaseCharacter::OnInputBufferConsumed);
    InputBuffer->OnInputBufferClose.RemoveDynamic(this, &ABaseCharacter::OnInputBufferClose);

    DynamicTargeting->OnTargetingToggled.RemoveDynamic(this, &ABaseCharacter::OnTargetingToggled);

    Equipment->OnActiveItemChanged.RemoveDynamic(this, &ABaseCharacter::OnActiveItemChanged);
    Equipment->OnCombatTypeChanged.RemoveDynamic(this, &ABaseCharacter::OnCombatTypeChanged);
    Equipment->OnMainHandTypeChanged.RemoveDynamic(this, &ABaseCharacter::OnMainHandTypeChanged);
    Equipment->OnInCombatChanged.RemoveDynamic(this, &ABaseCharacter::OnInCombatChanged);

    MovementSpeed->OnMovementStateStart.RemoveDynamic(this, &ABaseCharacter::OnMovementStateStart);
    MovementSpeed->OnMovementStateEnd.RemoveDynamic(this, &ABaseCharacter::OnMovementStateEnd);

    StateManager->OnStateChanged.RemoveDynamic(this, &ABaseCharacter::OnStateChanged);
    StateManager->OnActivityChanged.RemoveDynamic(this, &ABaseCharacter::OnActivityChanged);

    ExtendedHealth->OnValueChanged.RemoveDynamic(this, &ABaseCharacter::OnValueChanged_ExtendedHealth);
    ExtendedStamina->OnValueChanged.RemoveDynamic(this, &ABaseCharacter::OnValueChanged_ExtendedStamina);

    Rotating->OnRotatingEnd.RemoveDynamic(this, &ABaseCharacter::OnRotatingEnd);

    AbilityComponent->OnManaConsumed.RemoveDynamic(this, &ABaseCharacter::OnManaConsumed);
    AbilityComponent->OnAbilityStarted.RemoveDynamic(this, &ABaseCharacter::OnAbilityStarted);
    AbilityComponent->OnAbilityEnded.RemoveDynamic(this, &ABaseCharacter::OnAbilityEnded);
    AbilityComponent->OnAbilityChanged.RemoveDynamic(this, &ABaseCharacter::OnAbilityChanged);
    ExtendedMana->OnValueChanged.RemoveDynamic(this, &ABaseCharacter::OnValueChanged_ExtendedMana);

    
    StateManager = nullptr;
    StatsManager = nullptr;
    InputBuffer = nullptr;
    MeleeCollisionHandler = nullptr;
    MontageManager = nullptr;
    ExtendedHealth = nullptr;
    ExtendedStamina = nullptr;
    ExtendedMana = nullptr;
    DynamicTargeting = nullptr;
    Effects = nullptr;
    MovementSpeed = nullptr;
    Rotating = nullptr;
    Dissolve = nullptr;
    Equipment = nullptr;
    Inventory = nullptr;
    AbilityComponent = nullptr;
    FollowCamera = nullptr;
    CameraBoom = nullptr;
    EffectsAudio = nullptr;
    ArrowSpawnLocation = nullptr;
    TargetingArrow = nullptr;
    TargetWidget = nullptr;
    InGameWidget = nullptr;
    KeybindingsWidget = nullptr;
    PlayerOneHandMeleeMontages = nullptr;
    PlayerArcherMontages = nullptr;
    PlayerCommonMontages = nullptr;
    PlayerMagicMontages = nullptr;
    PlayerUnarmedMontages = nullptr;
    

    Super::EndPlay(EndPlayResult);
}

void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateAimAlpha();

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Zoom);
    PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ABaseCharacter::OnActionReleased_Zoom);
    PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Block);
    PlayerInputComponent->BindAction("Block", IE_Released, this, &ABaseCharacter::OnActionReleased_Block);

    PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Parry);
    PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SpecialAttack);
    PlayerInputComponent->BindAction("ThrustAttack", IE_Pressed, this, &ABaseCharacter::OnActionPressed_ThrustAttack);
    PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ABaseCharacter::OnActionPressed_HeavyAttack);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Attack);

    PlayerInputComponent->BindAction("TargetLeft", IE_Pressed, this, &ABaseCharacter::OnActionPressed_TargetLeft);
    PlayerInputComponent->BindAction("TargetRight", IE_Pressed, this, &ABaseCharacter::OnActionPressed_TargetRight);
    PlayerInputComponent->BindAction("ToggleTargeting", IE_Pressed, this, &ABaseCharacter::OnActionPressed_ToggleTargeting);

    PlayerInputComponent->BindAction("SwitchMainHandTypeUp", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SwitchMainHandTypeUp);
    PlayerInputComponent->BindAction("SwitchMainHandTypeDown", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SwitchMainHandTypeDown);
    PlayerInputComponent->BindAction("SwitchMainHandItemUp", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SwitchMainHandItemUp);
    PlayerInputComponent->BindAction("SwitchMainHandItemDown", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SwitchMainHandItemDown);
    PlayerInputComponent->BindAction("SwitchArrowsItem", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SwitchArrowsItem);
    PlayerInputComponent->BindAction("SwitchToolItem", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SwitchToolItem);
    PlayerInputComponent->BindAction("UseToolItem", IE_Pressed, this, &ABaseCharacter::OnActionPressed_UseToolItem);

    PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::OnAxis_MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::OnAxis_MoveRight);
    PlayerInputComponent->BindAxis("HorizontalLook", this, &ABaseCharacter::OnAxis_HorizontalLook);
    PlayerInputComponent->BindAxis("VerticalLook", this, &ABaseCharacter::OnAxis_VerticalLook);

    PlayerInputComponent->BindAction("ToggleMovement", IE_Pressed, this, &ABaseCharacter::OnActionPressed_ToggleMovement);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::OnActionReleased_Sprint);
    PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Roll);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::OnActionReleased_Jump);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Interact);
    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Inventory);
    PlayerInputComponent->BindAction("Equipment", IE_Pressed, this, &ABaseCharacter::OnActionPressed_Equipment);

    PlayerInputComponent->BindAction("ToggleCombat", IE_Pressed, this, &ABaseCharacter::OnActionPressed_ToggleCombat);
    PlayerInputComponent->BindAction("SlowMotion", IE_Pressed, this, &ABaseCharacter::OnActionPressed_SlowMotion);
    PlayerInputComponent->BindAction("SlowMotion", IE_Released, this, &ABaseCharacter::OnActionReleased_SlowMotion);

    PlayerInputComponent->BindAction("BowAttack", IE_Pressed, this, &ABaseCharacter::OnActionPressed_BowAttack);
    PlayerInputComponent->BindAction("BowAttack", IE_Released, this, &ABaseCharacter::OnActionReleased_BowAttack);

    PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_1);
    PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_2);
    PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_3);
    PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_4);
    PlayerInputComponent->BindKey(EKeys::Five, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_5);

    PlayerInputComponent->BindKey(EKeys::ThumbMouseButton, IE_Pressed, this, &ABaseCharacter::OnMousePressed_Thumb);
    PlayerInputComponent->BindKey(EKeys::ThumbMouseButton, IE_Released, this, &ABaseCharacter::OnMouseReleased_Thumb);

    PlayerInputComponent->BindAction("AbilityAttack", IE_Pressed, this, &ABaseCharacter::OnActionPressed_AbilityAttack);
    PlayerInputComponent->BindAction("AbilityAttack", IE_Released, this, &ABaseCharacter::OnActionReleased_AbilityAttack);

    PlayerInputComponent->BindKey(EKeys::P, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_P);
    PlayerInputComponent->BindKey(EKeys::K, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_K);
    PlayerInputComponent->BindKey(EKeys::K, IE_Released, this, &ABaseCharacter::OnKeyReleased_K);
    PlayerInputComponent->BindKey(EKeys::L, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_L);
    PlayerInputComponent->BindKey(EKeys::O, IE_Pressed, this, &ABaseCharacter::OnKeyPressed_O);
}

void ABaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
    EMovementMode NewMovementMode = GetCharacterMovement()->MovementMode;

    UpdateRotationSettings();
    UpdateZoom();

    AbilityOnMovementModeChanged(PrevMovementMode, NewMovementMode);

    StopBowDrawSound();
    if (NewMovementMode == EMovementMode::MOVE_Walking)
    {
        AttemptPlayBowDrawSound();
    }
}


EItemType ABaseCharacter::GetSelectedMainHandType() const
{
    return Equipment->GetSelectedMainHandType();
}

EState ABaseCharacter::GetState() const
{
    return StateManager->GetState();
}

void ABaseCharacter::CalculateLeanAmount(float& OutLeanAmount, float& OutInterpSpeed) const
{
    EMovementState MovementState = MovementSpeed->GetMovementState();

    bool bLean =
        IsIdleAndNotFalling() &&
        (MovementState == EMovementState::Jog || MovementState == EMovementState::Sprint) &&
        GetCharacterMovement()->Velocity.Size() > 10.0f &&
        !IsActivityEqual(EActivity::IsLookingForward);

    float HorizontalLookAxisVal = GetInputAxisValue("HorizontalLook");
    OutLeanAmount = bLean ? HorizontalLookAxisVal : 0.0f;
    OutInterpSpeed = bLean ? 1.0f : 10.0f;
}

bool ABaseCharacter::IsAlive() const
{
    return !IsStateEqual(EState::Dead);
}

FName ABaseCharacter::GetHeadSocket() const
{
    return FName("head");
}

bool ABaseCharacter::CanBeAttacked() const
{
    if (IsCharacterAlive())
    {
        return !IsActivityEqual(EActivity::IsImmortal);
    }
    else
    {
        return false;
    }
}

void ABaseCharacter::Block()
{
    UAnimMontage* AnimMontage = GetBlockMontage();

    if (GameUtils::IsValid(AnimMontage))
    {
        PlayAnimMontage(AnimMontage);
        ResetMeleeAttackCounter();
    }
}

bool ABaseCharacter::IsBlocked() const
{
    return GetReceivedHitDirection() == EDirection::Front && 
    GetBlockAlpha() >= 1.0f;
}

void ABaseCharacter::UpdateReceivedHitDirection(FVector InHitFromDirection)
{
    ReceivedHitDirection = UDefaultGameInstance::GetHitDirection(InHitFromDirection, this);
}

bool ABaseCharacter::CanBeStunned() const
{
    return !GetCharacterMovement()->IsFalling() && !Effects->IsEffectApplied(EEffectType::Backstab);
}

bool ABaseCharacter::CanBeInterrupted() const
{
    return !IsActivityEqual(EActivity::CantBeInterrupted);
}

bool ABaseCharacter::CanBeBackstabbed() const
{
    return CanBeStunned();
}

FRotator ABaseCharacter::GetDesiredRotation() const
{
    if (IsStateEqual(EState::Backstabbing))
    {
        if (BackstabbedActor.IsValid())
        {
            FRotator LookAtRot = 
                UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BackstabbedActor->GetActorLocation());

            float Yaw = LookAtRot.Yaw;
            return FRotator(GetActorRotation().Pitch, Yaw, GetActorRotation().Roll);
        }
    }

    if (AbilityComponent->IsCasting())
    {
        float Yaw = AbilityComponent->GetEffectTransform().Rotator().Yaw;
        return FRotator(GetActorRotation().Pitch, Yaw, GetActorRotation().Roll);
    }

    if (DynamicTargeting->IsTargetingEnabled())
    {
        float Yaw = UKismetMathLibrary::FindLookAtRotation(
            GetActorLocation(), DynamicTargeting->GetSelectedActorTransform().GetLocation()).Yaw;

        FRotator SampleRotation(GetActorRotation().Pitch, Yaw, GetActorRotation().Roll);

        if (HasMovementInput())
        {
            if (IsStateEqual(EState::Rolling))
            {
                if (MontageManager->GetLastRequestedAction() == EMontageAction::RollForward)
                {
                    return UKismetMathLibrary::Conv_VectorToRotator(GetLastMovementInputVector());
                }
            }

            return SampleRotation;
        }
        else
        {
            if (IsStateEqual(EState::Rolling))
            {

                if (MontageManager->GetLastRequestedAction() == EMontageAction::RollForward)
                {
                    return GetActorRotation();
                }
                else
                {
                    return SampleRotation;
                }
            }
            else
            {
                return SampleRotation;
            }
        }
    }
    else
    {
        if (HasMovementInput())
        {
            return UKismetMathLibrary::Conv_VectorToRotator(GetLastMovementInputVector());
        }
        else
        {
            return GetActorRotation();
        }
    }
}

UDataTable* ABaseCharacter::GetMontages(EMontageAction InAction) const
{
    TArray<EMontageAction> CommonMontages
    {
        EMontageAction::Backstabbed,
        EMontageAction::RollForward,
        EMontageAction::RollLeft,
        EMontageAction::RollRight,
        EMontageAction::RollBackward,
    };

    if (CommonMontages.Contains(InAction))
    {
        return PlayerCommonMontages;
    }
    else
    {
        if (Equipment->IsInCombat())
        {

        }
        else
        {
            TArray<EMontageAction> Array
            {
                EMontageAction::DisarmWeapon,
                EMontageAction::DrawWeapon
            };

            if (!Array.Contains(InAction))
            {
                return PlayerUnarmedMontages;
            }
        }

        if (Equipment->GetCombatType() == ECombatType::Melee)
        {
            return PlayerOneHandMeleeMontages;
        }
        else if (Equipment->GetCombatType() == ECombatType::Range)
        {
            return PlayerArcherMontages;
        }
        else if (Equipment->GetCombatType() == ECombatType::Magic)
        {
            return PlayerMagicMontages;
        }
        else
        {
            return PlayerUnarmedMontages;
        }
    }
}

bool ABaseCharacter::DoesHoldBowString() const
{
    return
        IsIdleAndNotFalling() &&
        Equipment->IsInCombat() &&
        IsCombatTypeEqual(ECombatType::Range) &&
        !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
}

float ABaseCharacter::BowAttack()
{
    if (CanBowAttack())
    {
        FStoredItem Item = Equipment->GetActiveItem(EItemType::Arrows, 0);
        SpawnArrowProjectile(Item);

        FGuid ItemId = Item.Id;
        int ItemIndex = Inventory->FindIndexById(ItemId);
        Inventory->RemoveItemAtIndex(ItemIndex, 1);

        StateManager->SetState(EState::Attacking);

        int MontageIndex = Equipment->AreArrowsEquipped() ? 0 : 1;
        UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(EMontageAction::ShootArrow, MontageIndex);

        if (GameUtils::IsValid(AnimMontage))
        {
            return PlayAnimMontage(AnimMontage);
        }
        else
        {
            StateManager->ResetState(0.0f);
        }
    }

    return 0.0f;
}

FTransform ABaseCharacter::GetSpawnArrowTransform() const
{
    FVector ArrowSpawnLoc = ArrowSpawnLocation->GetComponentLocation();
    FVector CameraDirection = FollowCamera->GetForwardVector();
    FRotator ArrowSpawnDirection = UKismetMathLibrary::Conv_VectorToRotator(CameraDirection);
    
    float VectorLength = (CameraBoom->GetComponentLocation() - FollowCamera->GetComponentLocation()).Size();
    const float TraceLength = 10000.0f;

    FVector LineTraceStart = FollowCamera->GetComponentLocation() + VectorLength * CameraDirection;
    FVector LineTraceEnd = FollowCamera->GetComponentLocation() + CameraDirection * TraceLength;

    for (int i = 0; i <= 20; ++i)
    {
        float Z = (float)i * 5.0f;
        FVector HeightVec = FVector(0.0f, 0.0f, Z);
        FVector LineTraceStartWithHeight = LineTraceStart - HeightVec;
        FVector LineTraceEndWithHeight = LineTraceEnd - HeightVec;
        FVector CurrentTraceDirection = UKismetMathLibrary::GetDirectionUnitVector(LineTraceStart, LineTraceEndWithHeight);

        // projectile
        ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
        TArray<AActor*> ActorsToIgnore;
        FHitResult OutHit;

        if (UKismetSystemLibrary::LineTraceSingle(
            GetWorld(),
            LineTraceStartWithHeight,
            LineTraceEndWithHeight,
            Channel,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::Type::None,
            OutHit,
            true))
        {
            FVector ImpactPoint = OutHit.ImpactPoint;
            AActor* HitActor = OutHit.GetActor();

            ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(HitActor);

            if (i == 0)
            {
                ArrowSpawnDirection = GetArrowSpawnDirection(
                    CameraDirection, CurrentTraceDirection, ImpactPoint, ArrowSpawnLoc);

                if (CanBeAttacked != nullptr)
                {
                    return FTransform(ArrowSpawnDirection, ArrowSpawnLoc);
                }
            }
            else
            {
                if ((ArrowSpawnLoc - ImpactPoint).Size() > 1000.0f)
                {
                    if (CanBeAttacked != nullptr)
                    {
                        ArrowSpawnDirection = GetArrowSpawnDirection(
                            CameraDirection, CurrentTraceDirection, ImpactPoint, ArrowSpawnLoc);

                        return FTransform(ArrowSpawnDirection, ArrowSpawnLoc);
                    }
                }
            }
        }
    }

    return FTransform(ArrowSpawnDirection, ArrowSpawnLoc);
}


void ABaseCharacter::OpenedUI()
{
    GetWorld()->GetTimerManager().ClearTimer(CheckForInteractableTimerHandle);
    InteractionActor = nullptr;
    InGameWidget->GetInteractionMessage()->UpdateWidget(NAME_None);
}

void ABaseCharacter::ClosedUI()
{
    GetWorld()->GetTimerManager().SetTimer(
        CheckForInteractableTimerHandle, this, &ABaseCharacter::CheckForInteractable, 0.1f, true);
}

bool ABaseCharacter::CanCastAbility() const
{
    return IsIdleAndNotFalling() && ExtendedMana->GetCurrentValue() > AbilityComponent->GetManaCost();
}

float ABaseCharacter::GetMagicDamage() const
{
    return StatsManager->GetStatValue(EStat::MagicDamage, true);
}

float ABaseCharacter::GetCastingSpeed() const
{
    return StatsManager->GetStatValue(EStat::CastingSpeed, true);
}

void ABaseCharacter::OnSelected()
{
    TargetWidget->SetHiddenInGame(false, false);
}

void ABaseCharacter::OnDeselected()
{
    TargetWidget->SetHiddenInGame(true, false);
}

bool ABaseCharacter::IsTargetable() const
{
    return IsAlive();
}

float ABaseCharacter::GetMeleeDamage() const
{
    return StatsManager->GetDamage();
}

bool ABaseCharacter::CanMeleeAttack() const
{
    return
        IsStateEqual(EState::Idle) &&
        Equipment->IsInCombat() &&
        (IsCombatTypeEqual(ECombatType::Unarmed) || IsCombatTypeEqual(ECombatType::Melee)) &&
        IsEnoughStamina(1.0f);
}

float ABaseCharacter::MeleeAttack(EMeleeAttackType InType)
{
    if (CanMeleeAttack())
    {
        MeleeAttackType = GetCharacterMovement()->IsFalling() ? EMeleeAttackType::Falling : InType;
        StateManager->SetState(EState::Attacking);
        GetWorld()->GetTimerManager().ClearTimer(ResetMeleeAttackCounterTimerHandle);

        UAnimMontage* AnimMontage = GetNextMeleeAttackMontage(MontageManager, MeleeAttackType);

        if (GameUtils::IsValid(AnimMontage))
        {
            float Value = StatsManager->GetStatValue(EStat::AttackSpeed, true);
            float Duration = PlayAnimMontage(AnimMontage, Value);
            float Time = Duration * 0.8f;

            GetWorld()->GetTimerManager().SetTimer(
                ResetMeleeAttackCounterTimerHandle, this, &ABaseCharacter::ResetMeleeAttackCounter, Time, false);

            float StaminaCost = StatsManager->GetStatValue(EStat::MeleeAttackStaminaCost, true);
            float FinalCost = UDefaultGameInstance::ScaleMeleeAttackStaminaCostByType(StaminaCost, MeleeAttackType);
            ExtendedStamina->ModifyStat(FinalCost * -1.0f, true);

            return Duration;
        }
        else
        {
            StateManager->ResetState(0.0f);
            ResetMeleeAttackCounter();
        }
    }
    return 0.0f;
}

void ABaseCharacter::OnEffectApplied(EEffectType InType)
{
    if (InType == EEffectType::Stun)
    {
        Stunned();
    }
    else if (InType == EEffectType::Backstab)
    {
        Backstabbed();
    }
    else if (InType == EEffectType::Impact)
    {

        Impact();
    }
    else if (InType == EEffectType::Parried)
    {
        Parried();
    }

    StateManager->SetState(EState::Disabled);
    InputBuffer->UpdateKey(EInputBufferKey::None);
    ResetMeleeAttackCounter();
    InputBuffer->OpenInputBuffer();
}

void ABaseCharacter::OnEffectRemoved(EEffectType InType)
{
    if (InType == EEffectType::Stun || 
        InType == EEffectType::Backstab || 
        InType == EEffectType::Impact ||
        InType == EEffectType::Parried)
    {
        TArray<EEffectType> Types
        {
            EEffectType::Stun, EEffectType::Knockdown, EEffectType::Impact, EEffectType::Parried, EEffectType::Backstab
        };


        if (!Effects->IsAnyEffectApplied(Types))
        {
            StateManager->ResetState(0.0f);
        }

        InputBuffer->CloseInputBuffer();
    }
}

void ABaseCharacter::OnInputBufferConsumed(EInputBufferKey InKey)
{
    FString EnumStr = GameUtils::GetEnumValueAsString("EInputBufferKey", InKey);

    if (IsCharacterAlive())
    {
        if (InKey == EInputBufferKey::LightAttack)
        {
            MeleeAttack(EMeleeAttackType::Light);
        }
        else if (InKey == EInputBufferKey::HeavyAttack)
        {
            MeleeAttack(EMeleeAttackType::Heavy);
        }
        else if (InKey == EInputBufferKey::ThrustAttack)
        {
            MeleeAttack(EMeleeAttackType::Thrust);
        }
        else if (InKey == EInputBufferKey::SpecialAttack)
        {
            MeleeAttack(EMeleeAttackType::Special);
        }
        else if (InKey == EInputBufferKey::FallingAttack)
        {
            MeleeAttack(EMeleeAttackType::Falling);
        }
        else if (InKey == EInputBufferKey::Roll)
        {
            Roll();
        }
        else if (InKey == EInputBufferKey::Jump)
        {
            CustomJump();
        }
        else if (InKey == EInputBufferKey::Parry)
        {
            Parry();
        }
        else if (InKey == EInputBufferKey::ToggleCombat)
        {
            ToggleCombat();
        }
        else if (InKey == EInputBufferKey::SwitchMainHandTypeUp)
        {
            if (CanUseOrSwitch())
            {
                Equipment->SwitchMainHandType(true);
            }
        }
        else if (InKey == EInputBufferKey::SwitchMainHandTypeDown)
        {
            if (CanUseOrSwitch())
            {
                Equipment->SwitchMainHandType(false);
            }
        }
        else if (InKey == EInputBufferKey::SwitchMainHandItemUp)
        {
            if (CanUseOrSwitch())
            {
                Equipment->SwitchSlotActiveIndex(Equipment->GetSelectedMainHandType(), 0, true, false);
            }
        }
        else if (InKey == EInputBufferKey::SwitchMainHandItemDown)
        {
            if (CanUseOrSwitch())
            {
                Equipment->SwitchSlotActiveIndex(Equipment->GetSelectedMainHandType(), 0, false, false);
            }
        }
        else if (InKey == EInputBufferKey::AbilityAttack)
        {
            AbilityPressed();
            if (!bIsAbilityInputPressed)
            {
                AbilityReleased();
            }
        }
        else if (InKey == EInputBufferKey::SetSpellActiveIndex)
        {
            SetSpellActiveIndex();
        }
    }
}

void ABaseCharacter::OnInputBufferClose()
{
    if (!IsStateEqual(EState::Disabled))
    {
        StateManager->ResetState(0.0f);
    }
}

void ABaseCharacter::OnActionPressed_Zoom()
{
    StartZooming();
}

void ABaseCharacter::OnActionReleased_Zoom()
{
    StopZooming();
}

void ABaseCharacter::OnActionPressed_Block()
{
    StartBlocking();
}

void ABaseCharacter::OnActionReleased_Block()
{
    StopBlocking();
}

void ABaseCharacter::OnActionPressed_Parry()
{
    if (IsCombatTypeEqual(ECombatType::Unarmed) || IsCombatTypeEqual(ECombatType::Melee))
    {
        InputBuffer->UpdateKey(EInputBufferKey::Parry);
    }
}

void ABaseCharacter::OnActionPressed_SpecialAttack()
{
    if (IsCombatTypeEqual(ECombatType::Melee))
    {
        InputBuffer->UpdateKey(EInputBufferKey::SpecialAttack);
    }
}

void ABaseCharacter::OnActionPressed_ThrustAttack()
{
    if (IsCombatTypeEqual(ECombatType::Melee))
    {
        InputBuffer->UpdateKey(EInputBufferKey::ThrustAttack);
    }
}

void ABaseCharacter::OnActionPressed_HeavyAttack()
{
    if (IsCombatTypeEqual(ECombatType::Unarmed) || IsCombatTypeEqual(ECombatType::Melee))
    {
        InputBuffer->UpdateKey(EInputBufferKey::HeavyAttack);
    }
}

void ABaseCharacter::OnActionPressed_Attack()
{
    if (Equipment->IsInCombat())
    {
        if (IsCombatTypeEqual(ECombatType::Melee))
        {
            if (!AttemptBackstab())
            {
                InputBuffer->UpdateKey(EInputBufferKey::LightAttack);
            }
        }
        else
        {
            if (IsCombatTypeEqual(ECombatType::Unarmed))
            {
                InputBuffer->UpdateKey(EInputBufferKey::LightAttack);
            }
        }
    }
    else
    {
        InputBuffer->UpdateKey(EInputBufferKey::ToggleCombat);
    }
}

void ABaseCharacter::OnActionPressed_ToggleTargeting()
{
    if (IsCharacterAlive())
    {
        DynamicTargeting->ToggleCameraLock();
    }
}

void ABaseCharacter::OnActionPressed_TargetLeft()
{
    DynamicTargeting->FindTargetOnLeft();
}

void ABaseCharacter::OnActionPressed_TargetRight()
{
    DynamicTargeting->FindTargetOnRight();
}

void ABaseCharacter::OnTargetingToggled(bool bInEnabled)
{
    UpdateRotationSettings();
}

void ABaseCharacter::OnActiveItemChanged(
    const FStoredItem& OldItem, 
    const FStoredItem& NewItem,
    EItemType Type,
    int SlotIndex,
    int ActiveIndex)
{
    PlayMainHandTypeChangedMontage(Type);
    if (Type == EItemType::Arrows)
    {
        if (!Equipment->AreArrowsEquipped() &&
            Equipment->IsInCombat() &&
            IsCombatTypeEqual(ECombatType::Range))
        {
            ResetAimingMode();
        }
    }
}

void ABaseCharacter::OnCombatTypeChanged(ECombatType CombatType)
{
    if (!AbilityComponent->IsPressed())
    {
        ResetAimingMode();
    }
}

void ABaseCharacter::OnMainHandTypeChanged(EItemType InType)
{
    PlayMainHandTypeChangedMontage(InType);
}

void ABaseCharacter::OnInCombatChanged(bool bIsInCombat)
{
    UpdateRotationSettings();
    if (!bIsInCombat)
    {
        StopBlocking();
        ResetAimingMode();
    }
}

void ABaseCharacter::OnActionPressed_SwitchMainHandTypeUp()
{
    InputBuffer->UpdateKey(EInputBufferKey::SwitchMainHandTypeUp);
}

void ABaseCharacter::OnActionPressed_SwitchMainHandTypeDown()
{
    InputBuffer->UpdateKey(EInputBufferKey::SwitchMainHandTypeDown);
}

void ABaseCharacter::OnActionPressed_SwitchMainHandItemUp()
{
    InputBuffer->UpdateKey(EInputBufferKey::SwitchMainHandItemUp);
}

void ABaseCharacter::OnActionPressed_SwitchMainHandItemDown()
{
    InputBuffer->UpdateKey(EInputBufferKey::SwitchMainHandItemDown);
}

void ABaseCharacter::OnActionPressed_SwitchArrowsItem()
{
    if (CanUseOrSwitch())
    {
        Equipment->SwitchSlotActiveIndex(EItemType::Arrows, 0, true, true);
    }
}

void ABaseCharacter::OnActionPressed_SwitchToolItem()
{
    if (CanUseOrSwitch())
    {
        Equipment->SwitchSlotActiveIndex(EItemType::Tool, 0, true, true);
    }
}

void ABaseCharacter::OnActionPressed_UseToolItem()
{
    UseItem(EItemType::Tool);
}

void ABaseCharacter::OnAxis_MoveForward(float AxisValue)
{
    FVector OutForwardVector;
    FVector OutRightVector;
    GetMovementVectors(OutForwardVector, OutRightVector);

    if (IsCharacterAlive())
    {
        AddMovementInput(OutForwardVector, AxisValue, false);
    }
}

void ABaseCharacter::OnAxis_MoveRight(float AxisValue)
{
    FVector OutForwardVector;
    FVector OutRightVector;
    GetMovementVectors(OutForwardVector, OutRightVector);

    if (IsCharacterAlive())
    {
        AddMovementInput(OutRightVector, AxisValue, false);
    }
}

void ABaseCharacter::OnAxis_HorizontalLook(float AxisValue)
{
    float Val = AxisValue * HorizontalLookRate * GetWorld()->GetDeltaSeconds();
    AddControllerYawInput(Val);

    float NewVal = GetInputAxisValue("HorizontalLook");
    DynamicTargeting->FindTargetWithAxisInput(NewVal);
}

void ABaseCharacter::OnAxis_VerticalLook(float AxisValue)
{
    float Val = AxisValue * VerticalLookRate * GetWorld()->GetDeltaSeconds();
    AddControllerPitchInput(Val);
}

void ABaseCharacter::OnActionPressed_ToggleMovement()
{
    MovementSpeed->ToggleState();
}

void ABaseCharacter::OnActionPressed_Sprint()
{
    SetSprint(true);
}

void ABaseCharacter::OnActionReleased_Sprint()
{
    SetSprint(false);
}

void ABaseCharacter::OnMovementStateStart(EMovementState InState)
{
    if (InState == EMovementState::Sprint)
    {
        GetWorld()->GetTimerManager().SetTimer(SprintLoopTimerHandle, this, &ABaseCharacter::SprintLoop, 0.016f, true);
    }
}

void ABaseCharacter::OnMovementStateEnd(EMovementState InState)
{
    if (InState == EMovementState::Sprint)
    {
        GetWorld()->GetTimerManager().ClearTimer(SprintLoopTimerHandle);
    }
}

void ABaseCharacter::OnActionPressed_Roll()
{
    InputBuffer->UpdateKey(EInputBufferKey::Roll);
}

void ABaseCharacter::OnActionPressed_Jump()
{
    InputBuffer->UpdateKey(EInputBufferKey::Jump);
}

void ABaseCharacter::OnActionReleased_Jump()
{
    StopJumping();
}

void ABaseCharacter::OnStateChanged(EState PrevState, EState NewState)
{
    UpdateBlocking();

    UpdateZoom();

    UpdateRotationSettings();

    AbilityOnStateChanged(NewState);

    if (PrevState == EState::Attacking)
    {
        MeleeAttackType = EMeleeAttackType::None;
        BackstabbedActor = nullptr;
        StopBowDrawSound();
        if (NewState == EState::Idle)
        {
            AttemptPlayBowDrawSound();
        }
    }
}

void ABaseCharacter::OnActionPressed_Interact()
{
    if (CanOpenUI())
    {
        if (InteractionActor.IsValid())
        {
            IIsInteractable* IsInteractable = Cast<IIsInteractable>(InteractionActor.Get());

            if (IsInteractable != nullptr)
            {
                IsInteractable->Interact(this);
            }
        }
    }
}

void ABaseCharacter::CheckForInteractable()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * 100.0f;
    const float Radius = 20.0f;
    const float HalfRadius = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    // Interactable == ECC_GameTraceChannel2  
    // defined DefaultEngine.ini
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes
    { 
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2) 
    };

    TArray<AActor*> ActorsToIgnore;

    FHitResult HitResult;
    if (UKismetSystemLibrary::CapsuleTraceSingleForObjects(
        GetWorld(),
        Start,
        End, Radius, HalfRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::Type::None, HitResult, true))
    {
        if (InteractionActor.Get() != HitResult.GetActor())
        {
            InteractionActor = HitResult.GetActor();

            IIsInteractable* IsInteractable = Cast<IIsInteractable>(InteractionActor.Get());
            if (IsInteractable != nullptr)
            {
                FName Message = IsInteractable->GetInteractionMessage();
                InGameWidget->GetInteractionMessage()->UpdateWidget(Message);
            }
        }
    }
    else
    {
        if (InteractionActor.IsValid())
        {
            InteractionActor = nullptr;
            InGameWidget->GetInteractionMessage()->UpdateWidget(TEXT(""));
        }
    }

}

void ABaseCharacter::OnActivityChanged(EActivity Activity, bool bValue)
{
    if (Activity == EActivity::IsBlockingPressed)
    {
        UpdateBlocking();
    }
    else if (Activity == EActivity::IsLookingForward)
    {
        UpdateRotationSettings();
    }
    else if (Activity == EActivity::IsZooming)
    {
        UpdateZoom();
    }
}

void ABaseCharacter::OnActionPressed_Inventory()
{
    if (CanOpenUI())
    {
        InGameWidget->GetInventory()->Open();
    }
}

void ABaseCharacter::OnActionPressed_Equipment()
{
    if (CanOpenUI())
    {
        InGameWidget->GetEquipment()->Open();
    }
}

void ABaseCharacter::OnValueChanged_ExtendedHealth(float InNewValue, float InMaxValue)
{
    if (InNewValue <= 0.0f)
    {
        Death();
    }
}

void ABaseCharacter::OnValueChanged_ExtendedStamina(float InNewValue, float InMaxValue)
{
    if (InNewValue / InMaxValue <= 0.1f)
    {
        UpdateBlocking();
    }
}

void ABaseCharacter::OnRotatingEnd()
{
    if (IsStateEqual(EState::Attacking) &&
        (IsCombatTypeEqual(ECombatType::Unarmed) || IsCombatTypeEqual(ECombatType::Melee)) &&
        DynamicTargeting->IsTargetingEnabled() &&
        Equipment->IsInCombat())
    {
        Rotating->StartRotatingWithTime(0.5f, 720.0f);
    }
}

void ABaseCharacter::OnActionPressed_ToggleCombat()
{
    if (AbilityComponent->IsUsingAbility())
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Canceled);
        if (!AbilityComponent->IsUsingAbility())
        {
            ResetAimingMode();
            bIsAbilityInputPressed = false;
            bIsAbilityMainInputPressed = false;
        }
    }
    else
    {
        if (IsActivityEqual(EActivity::IsAimingPressed) && IsIdleAndNotFalling())
        {
            ResetAimingMode();
            StopBowDrawSound();
        }
        else
        {
            InputBuffer->UpdateKey(EInputBufferKey::ToggleCombat);
        }
    }
}

void ABaseCharacter::OnActionPressed_SlowMotion()
{
    StartSlowMotion();
}

void ABaseCharacter::OnActionReleased_SlowMotion()
{
    StopSlotMotion();
}

void ABaseCharacter::OnActionPressed_BowAttack()
{
    if (Equipment->IsInCombat())
    {
        if (CanBowAttack())
        {
            StartAiming();
            StartLookingForward();
            UpdateZoom();
            ShowCrosshair(GameUtils::GetDefaultGameInstance(GetWorld())->CrosshairTexture);
            AttemptPlayBowDrawSound();
        }
    }
    else
    {
        InputBuffer->UpdateKey(EInputBufferKey::ToggleCombat);
    }
}

void ABaseCharacter::OnActionReleased_BowAttack()
{
    if (CanBowAttack())
    {
        StopAiming();
        if (AimAlpha >= 0.8f)
        {
            BowAttack();
        }

        GetWorld()->GetTimerManager().SetTimer(
            StopLookingForwardTimerHandle, this, &ABaseCharacter::StopLookingForward, 0.8f, false);

        GetWorld()->GetTimerManager().SetTimer(
            HideCrosshairTimerHandle, this, &ABaseCharacter::HideCrosshair, 0.8f, false);

        GetWorld()->GetTimerManager().SetTimer(
            UpdateZoomTimerHandle, this, &ABaseCharacter::UpdateZoom, 0.81f, false);
        StopBowDrawSound();
    }
}

void ABaseCharacter::OnKeyPressed_1()
{
    UpdateSpellActiveIndexKey(0);
}

void ABaseCharacter::OnKeyPressed_2()
{
    UpdateSpellActiveIndexKey(1);
}

void ABaseCharacter::OnKeyPressed_3()
{
    UpdateSpellActiveIndexKey(2);
}

void ABaseCharacter::OnKeyPressed_4()
{
    UpdateSpellActiveIndexKey(3);
}

void ABaseCharacter::OnKeyPressed_5()
{
    UpdateSpellActiveIndexKey(4);
}

void ABaseCharacter::OnManaConsumed(float Amount)
{
    ExtendedMana->ModifyStat(Amount * -1.0f, true);
}

void ABaseCharacter::OnValueChanged_ExtendedMana(float NewValue, float MaxValue)
{
    if (NewValue < AbilityComponent->GetManaCost())
    {
        AbilityComponent->EndAbility(EAbilityEndResult::OutOfMana);
    }
    else
    {
        if (bIsAbilityInputPressed)
        {
            if (AbilityComponent->IsPressed())
            {
                UpdateAbilityPressed();
            }
        }
    }
}

void ABaseCharacter::OnAbilityStarted()
{
    if (!AbilityComponent->CanAbilityBeCancelled())
    {
        StateManager->SetState(EState::Attacking);
    }
}

void ABaseCharacter::OnAbilityEnded(EAbilityEndResult Result)
{
    if (IsStateEqual(EState::Attacking))
    {
        StateManager->ResetState(0.0f);
    }

    if (!AbilityComponent->IsPressed())
    {
        DisableAbilityMode();
    }
}

void ABaseCharacter::OnMousePressed_Thumb()
{
    if (!bIsAbilityInputPressed)
    {
        bIsAbilityInputPressed = true;
        UpdateAbilityAttackKey();
    }
}

void ABaseCharacter::OnMouseReleased_Thumb()
{
    if (bIsAbilityInputPressed)
    {
        bIsAbilityInputPressed = false;
        AbilityReleased();
    }
}

void ABaseCharacter::OnAbilityChanged(AAbilityBase* const NewAbility)
{
    UpdateAbilityCrosshair();

    if (NewAbility != nullptr)
    {
        ResetAimingMode();
    }
}

void ABaseCharacter::OnActionPressed_AbilityAttack()
{
    if (IsCombatTypeEqual(ECombatType::Magic) && !bIsAbilityInputPressed)
    {
        bIsAbilityInputPressed = true;
        bIsAbilityMainInputPressed = true;
        UpdateAbilityAttackKey();
    }
}

void ABaseCharacter::OnActionReleased_AbilityAttack()
{
    if ((IsCombatTypeEqual(ECombatType::Magic) || bIsAbilityMainInputPressed) && bIsAbilityInputPressed)
    {
        bIsAbilityInputPressed = false;
        bIsAbilityMainInputPressed = false;
        AbilityReleased();
    }
}

void ABaseCharacter::OnKeyPressed_P()
{
    // save game
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    GameMode->SaveGame();
}

void ABaseCharacter::OnKeyPressed_K()
{
    KeybindingsWidget->ShowKeybindings();
}

void ABaseCharacter::OnKeyReleased_K()
{
    KeybindingsWidget->HideKeybindings();
}

void ABaseCharacter::OnKeyPressed_L()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), FString(TEXT("RestartLevel")), PlayerController);
}

void ABaseCharacter::OnKeyPressed_O()
{
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    GameMode->LoadGame();
}

void ABaseCharacter::CreateKeybindings()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    KeybindingsWidget = Cast<UKeybindingsUI>(
        CreateWidget(PlayerController, GameUtils::GetDefaultGameInstance(GetWorld())->KeybindingsUIClass));
    KeybindingsWidget->AddToViewport(1);
}

void ABaseCharacter::AbilityPressed()
{
    if (AbilityComponent->IsCurrentAbilityValid() &&
        Equipment->IsInCombat() &&
        (ExtendedMana->GetCurrentValue() > AbilityComponent->GetManaCost()))
    {
        AbilityComponent->AbilityPressed();
        EnableAbilityMode();
    }
}

void ABaseCharacter::AbilityReleased()
{
    if (AbilityComponent->IsPressed())
    {
        AbilityComponent->AbilityReleased();

        if (!AbilityComponent->IsCasting())
        {
            DisableAbilityMode();
        }
    }
}

void ABaseCharacter::UpdateSpellActiveIndexKey(int InNewActiveIndex)
{
    SelectedSpellIndex = InNewActiveIndex;
    InputBuffer->UpdateKey(EInputBufferKey::SetSpellActiveIndex);
}

void ABaseCharacter::UpdateAbilityCrosshair()
{
    if (AbilityComponent->IsAbilityUsingCrosshair())
    {
        if (AbilityComponent->IsPressed())
        {
            ShowCrosshair(AbilityComponent->GetAbilityCrosshair());
        }
        else
        {
            HideCrosshair();
        }
    }
    else
    {
        HideCrosshair();
    }
}

void ABaseCharacter::UpdateAbilityPressed()
{
    if (bIsAbilityInputPressed && InputBuffer->GetStoredKey() == EInputBufferKey::None)
    {
        UpdateAbilityAttackKey();
    }
}

void ABaseCharacter::UpdateZoom()
{
    bool Or1 = (AbilityComponent->IsUsingAbility() && AbilityComponent->ShouldRotateOnPressed()) ||
        IsActivityEqual(EActivity::IsLookingForward);

    bool Or2 = IsIdleAndNotFalling() && IsStateEqual(EState::Attacking);
    bool Or3 = IsActivityEqual(EActivity::IsZooming) && bAutoZoom;
    bool bCondition = Or1 && Or2 && Or3;

    UpdateZoomTimeline(bCondition);
}

void ABaseCharacter::UpdateBlocking()
{
    bool bCondition =
        IsActivityEqual(EActivity::IsBlockingPressed) &&
        IsIdleAndNotFalling() &&
        Equipment->CanBlock() &&
        IsEnoughStamina(5.0f);

    UpdateBlockingTimeline(bCondition);
}

void ABaseCharacter::PlayMainHandTypeChangedMontage(EItemType InType)
{
    if (Equipment->GetSelectedMainHandType() == InType && Equipment->IsInCombat())
    {
        StateManager->SetState(EState::Interacting);
        UAnimMontage* Montage = MontageManager->GetMontageForAction(EMontageAction::DrawWeapon, 1);
        PlayAnimMontage(Montage, 1.0f);
        StateManager->ResetState(0.1f);
    }
}

void ABaseCharacter::StartLookingForward()
{
    StateManager->SetActivity(EActivity::IsLookingForward, true);
    GetWorld()->GetTimerManager().ClearTimer(StopLookingForwardTimerHandle);
}

void ABaseCharacter::StopLookingForward()
{
    StateManager->SetActivity(EActivity::IsLookingForward, false);
}

void ABaseCharacter::StartAiming()
{
    StateManager->SetActivity(EActivity::IsAimingPressed, true);
}

void ABaseCharacter::StopAiming()
{
    StateManager->SetActivity(EActivity::IsAimingPressed, false);
}

void ABaseCharacter::StartBlocking()
{
    StateManager->SetActivity(EActivity::IsBlockingPressed, true);
}

void ABaseCharacter::StopBlocking()
{
    StateManager->SetActivity(EActivity::IsBlockingPressed, false);
}

void ABaseCharacter::StartZooming()
{
    StateManager->SetActivity(EActivity::IsZooming, true);
}

void ABaseCharacter::StopZooming()
{
    StateManager->SetActivity(EActivity::IsZooming, false);
}

void ABaseCharacter::PlayBowDrawSound()
{
    if (IsIdleAndNotFalling() && CanBowAttack() && IsActivityEqual(EActivity::IsAimingPressed))
    {
        EffectsAudio->SetSound(nullptr);
        EffectsAudio->Play();
        GetWorld()->GetTimerManager().ClearTimer(PlayBowDrawSoundTimerHandle);
    }
}

void ABaseCharacter::AttemptPlayBowDrawSound()
{
    GetWorld()->GetTimerManager().SetTimer(
        PlayBowDrawSoundTimerHandle, this, &ABaseCharacter::PlayBowDrawSound, 0.1f, true);

    GameUtils::SetTimerRetriggerable(
        GetWorld()->GetTimerManager(),
        RetriggerableDelayTimerHandle,
        FTimerDelegate::CreateUObject(this, &ABaseCharacter::AttemptPlayBowDrawSoundDelayed),
        1.0f,
        true);
}

void ABaseCharacter::AttemptPlayBowDrawSoundDelayed()
{
    GetWorld()->GetTimerManager().ClearTimer(PlayBowDrawSoundTimerHandle);
}

void ABaseCharacter::StopBowDrawSound()
{
    GetWorld()->GetTimerManager().ClearTimer(PlayBowDrawSoundTimerHandle);
    EffectsAudio->Stop();
}

void ABaseCharacter::UpdateAbilityAttackKey()
{
    EInputBufferKey Key = Equipment->IsInCombat() ? EInputBufferKey::AbilityAttack : EInputBufferKey::ToggleCombat;

    InputBuffer->UpdateKey(Key);
}

void ABaseCharacter::EnableAbilityMode()
{
    StopAiming();

    if (AbilityComponent->ShouldRotateOnPressed())
    {
        StartLookingForward();
    }
    else
    {
        StopLookingForward();
    }

    UpdateZoom();
    UpdateAbilityCrosshair();
}

void ABaseCharacter::DisableAbilityMode()
{
    if (!AbilityComponent->IsCasting() && !AbilityComponent->IsPressed())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StopLookingForwardTimerHandle, this, &ABaseCharacter::StopLookingForward, 1.0f, false);

        GetWorld()->GetTimerManager().SetTimer(
            HideCrosshairTimerHandle, this, &ABaseCharacter::HideCrosshair, 1.0f, false);

        GetWorld()->GetTimerManager().SetTimer(
            UpdateZoomTimerHandle, this, &ABaseCharacter::UpdateZoom, 1.1f, false);

    }
}

void ABaseCharacter::AbilityOnStateChanged(EState NewState)
{
    if (EState::Idle != NewState &&
        EState::Attacking != NewState)
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
    }
    else
    {
        if (EState::Idle == NewState)
        {
            UpdateAbilityPressed();
        }
    }
}

void ABaseCharacter::AbilityOnMovementModeChanged(EMovementMode PrevMovementMode, EMovementMode NewMovementMode)
{
    if (NewMovementMode == EMovementMode::MOVE_Walking)
    {
        UpdateAbilityPressed();
    }
    else
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
    }
}

void ABaseCharacter::SetSpellActiveIndex()
{
    if (CanUseOrSwitch())
    {
        Equipment->SetMainHandType(EItemType::Spell);
        Equipment->SetSlotActiveIndex(EItemType::Spell, 0, SelectedSpellIndex);
    }
}

UAnimMontage* ABaseCharacter::GetRollMontage() const
{
    EMontageAction RollDirection;

    if (!HasMovementInput())
    {
        RollDirection = EMontageAction::RollBackward;

        UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(RollDirection, 0);
        if (GameUtils::IsValid(AnimMontage))
        {
            return AnimMontage;
        }
    }

    RollDirection = EMontageAction::RollForward;
    return MontageManager->GetMontageForAction(RollDirection, 0);
}

FRotator ABaseCharacter::GetArrowSpawnDirection(
    FVector InCameraDirection,
    FVector InCurrentTraceDirection,
    FVector InImpactPoint,
    FVector InArrowSpawnLocation) const
{
    float Dot = FVector::DotProduct(InCameraDirection.GetSafeNormal(), InCurrentTraceDirection.GetSafeNormal());
    
    // length to point on camera direction line above impact point
    float Val = (FollowCamera->GetComponentLocation() - InImpactPoint).Size() / 
        UKismetMathLibrary::DegCos(UKismetMathLibrary::DegAcos(Dot));
    
    FVector From = InArrowSpawnLocation;
    FVector To = FollowCamera->GetComponentLocation() + InCameraDirection * Val;
    
    return UKismetMathLibrary::Conv_VectorToRotator(UKismetMathLibrary::GetDirectionUnitVector(From, To));
}

void ABaseCharacter::Roll()
{
    if (CanRoll())
    {
        StateManager->SetState(EState::Rolling);

        UAnimMontage* AnimMontage = GetRollMontage();

        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAnimMontage(AnimMontage);
            ExtendedStamina->ModifyStat(RollStaminaCost * -1.0f, true);
        }
        else
        {
            StateManager->ResetState(0.0f);
        }
    }
}

void ABaseCharacter::CustomJump()
{
    if (IsStateEqual(EState::Idle) && CanJump())
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        if (AnimInstance->IsAnyMontagePlaying())
        {
            AnimInstance->Montage_Stop(0.25f);
        }
        Jump();
    }
}

void ABaseCharacter::ToggleCombat()
{
    EState CurrentState = StateManager->GetState();
    FString EnumStr = GameUtils::GetEnumValueAsString(TEXT("EState"), CurrentState);

    if (IsStateEqual(EState::Idle))
    {
        StateManager->SetState(EState::Interacting);
        EMontageAction Action = Equipment->IsInCombat() ? EMontageAction::DisarmWeapon : EMontageAction::DrawWeapon;
        UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(Action, 0);

        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAnimMontage(AnimMontage);
        }
        else
        {
            Equipment->ToggleCombat();
            StateManager->ResetState(0.0f);
        }
    }
}

void ABaseCharacter::Parry()
{
    if (CanMeleeAttack() && !GetCharacterMovement()->IsFalling())
    {
        StateManager->SetState(EState::Parrying);
        UAnimMontage* AnimMontage = GetParryMontage();

        if (GameUtils::IsValid(AnimMontage))
        {
            float Value = StatsManager->GetStatValue(EStat::AttackSpeed, true);
            PlayAnimMontage(AnimMontage, Value);
            ExtendedStamina->ModifyStat(-15.0f, true);
        }
        else
        {
            StateManager->ResetState(0.0f);
        }
    }
}

bool ABaseCharacter::AttemptBackstab()
{
    if (CanMeleeAttack())
    {
        FVector Start = GetActorLocation();
        FVector End = Start + GetActorForwardVector() * 150.0f;


        // projectile
        ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
        TArray<AActor*> ActorsToIgnore;
        FHitResult OutHit;

        if (UKismetSystemLibrary::LineTraceSingle(
            GetWorld(), Start, End, Channel, false, ActorsToIgnore, EDrawDebugTrace::Type::None, OutHit, true))
        {
            AActor* HitActor = OutHit.GetActor();
            if (HitActor->GetDotProductTo(this) < -0.25f)
            {
                UEffectsComponent* EffectsComponent = 
                    Cast<UEffectsComponent>(HitActor->GetComponentByClass(UEffectsComponent::StaticClass()));

                if (GameUtils::IsValid(EffectsComponent))
                {
                    bool bApplied = EffectsComponent->ApplyBackstabEffect(
                        1.0f, EApplyEffectMethod::Replace, this, StatsManager->GetDamage() * 3.0f);

                    if (bApplied)
                    {
                        BackstabbedActor = HitActor;
                        StateManager->SetState(EState::Backstabbing);
                        UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(EMontageAction::Backstab, 0);

                        if (GameUtils::IsValid(AnimMontage))
                        {
                            PlayAnimMontage(AnimMontage);
                            ExtendedStamina->ModifyStat(-40.0f, true);
                            return true;
                        }
                        else
                        {
                            StateManager->ResetState(0.0f);
                            return false;
                        }
                    }
                }
            }
        }
    }

    return false;
}

void ABaseCharacter::UseItem(EItemType InType)
{
    if (CanUseOrSwitch())
    {
        FStoredItem ActiveItem = Equipment->GetActiveItem(InType, 0);

        if(UKismetSystemLibrary::IsValidClass(ActiveItem.ItemClass))
        {
            UItemBase* ItemBase = NewObject<UItemBase>(this, ActiveItem.ItemClass);

            IItemHasUseMontage* ItemHasUseMontage = Cast<IItemHasUseMontage>(ItemBase);
            if (ItemHasUseMontage != nullptr)
            {
                UAnimMontage* AnimMontage = ItemHasUseMontage->GetUseMontage();

                if (GameUtils::IsValid(AnimMontage))
                {
                    StateManager->SetState(EState::Interacting);
                    float Duration = PlayAnimMontage(AnimMontage);
                    StateManager->ResetState(Duration * 0.8f);
                }
                else
                {
                    Inventory->UseItem(ActiveItem.Id);
                }
            }
            else
            {
                Inventory->UseItem(ActiveItem.Id);                
            }
        }
    }
}

bool ABaseCharacter::CanRoll() const
{
    return IsIdleAndNotFalling() && IsEnoughStamina(1.0f);
}

bool ABaseCharacter::CanUseOrSwitchItem() const
{
    if (IsCharacterAlive())
    {
        return IsStateEqual(EState::Idle);
    }
    else
    {
        return false;
    }
}

bool ABaseCharacter::CanOpenUI() const
{
    if (IsCharacterAlive())
    {
        return IsStateEqual(EState::Idle);
    }
    else
    {
        return false;
    }
}

bool ABaseCharacter::CanEnterSlowMotion() const
{
    return IsActivityEqual(EActivity::IsLookingForward) && (IsStateEqual(EState::Attacking) || IsIdleAndNotFalling());
}



void ABaseCharacter::ResetAimingMode()
{
    StopLookingForward();
    StopAiming();
    StopZooming();
    HideCrosshair();
}

void ABaseCharacter::ShowCrosshair(UTexture2D* const InTexture)
{
    GetWorld()->GetTimerManager().ClearTimer(HideCrosshairTimerHandle);

    if (GameUtils::IsValid(InGameWidget))
    {
        InGameWidget->GetCrosshair()->SetVisibility(ESlateVisibility::Visible);

        UTexture2D* FinalTexture = 
            UKismetSystemLibrary::IsValid(InTexture) ? 
            InTexture : GameUtils::GetDefaultGameInstance(GetWorld())->DefaultCrosshairTextureObject;
        InGameWidget->GetCrosshair()->SetBrushFromTexture(FinalTexture, true);
    }
}

void ABaseCharacter::HideCrosshair()
{
    if (GameUtils::IsValid(InGameWidget))
    {
        InGameWidget->GetCrosshair()->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ABaseCharacter::UpdateAimAlpha()
{
    float DeltaTime = GetWorld()->GetDeltaSeconds();

    if (IsActivityEqual(EActivity::IsAimingPressed) && IsIdleAndNotFalling())
    {
        float Value = StatsManager->GetStatValue(EStat::AttackSpeed, true);
        AimAlpha = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 1.0f, DeltaTime, Value);
    }
    else
    {
        AimAlpha = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 0.0f, DeltaTime, 3.0f);
    }
}

void ABaseCharacter::UpdateRotationSettings()
{
    if (IsActivityEqual(EActivity::IsLookingForward) &&
           (IsIdleAndNotFalling() ||
            IsStateEqual(EState::Attacking) ||
            IsStateEqual(EState::Parrying) ||
            IsStateEqual(EState::Backstabbing) ||
            (AbilityComponent->IsPressed() && !GetCharacterMovement()->IsFalling())))
    {
        VerticalLookRate = 25.0f;
        HorizontalLookRate = 25.0f;
        SetCameraLagSmoothly(50.0f);

        DynamicTargeting->SetFreeCamera(true);
        Rotating->SetRotationMode(ERotationMode::FaceCamera);
    }
    else
    {
        VerticalLookRate = 45.0f;
        HorizontalLookRate = 45.0f;

        SetCameraLagSmoothly(InitialCameraLagSpeed);

        if (DynamicTargeting->IsTargetingEnabled())
        {
            if (Equipment->IsInCombat())
            {
                // set free camera and rotation mode (only if not in roll) based on combat type
                bool bFreeCamera;
                if (Equipment->GetCombatType() == ECombatType::Range ||
                    Equipment->GetCombatType() == ECombatType::Magic)
                {
                    bFreeCamera = true;
                }
                else
                {
                    bFreeCamera = false;
                }

                DynamicTargeting->SetFreeCamera(bFreeCamera);

                ERotationMode Mode = ERotationMode::FaceCamera;
                if (Equipment->GetCombatType() == ECombatType::Range || 
                    Equipment->GetCombatType() == ECombatType::Magic)
                {
                    Mode = ERotationMode::OrientToMovement;
                }
                Rotating->SetRotationMode(Mode);
            }
            else
            {
                // free camera and update rotation mode only if state is not equal roll 
                // (because roll should use OrientToMovement)
                DynamicTargeting->SetFreeCamera(false);
                Rotating->SetRotationMode(ERotationMode::FaceCamera);
            }
        }
        else
        {
            Rotating->SetRotationMode(ERotationMode::OrientToMovement);
        }
    }
}

void ABaseCharacter::StartSlowMotion()
{
    if (CanEnterSlowMotion())
    {
        bIsInSlowMotion = true;
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMotionTimeDilation);

        GetWorld()->GetTimerManager().SetTimer(
            LoopSlowMotionTimerHandle, this, &ABaseCharacter::LoopSlowMotion, 0.016f, true);
    }

}

void ABaseCharacter::StopSlotMotion()
{
    bIsInSlowMotion = false;
    
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    GetWorld()->GetTimerManager().ClearTimer(LoopSlowMotionTimerHandle);
}

void ABaseCharacter::LoopSlowMotion()
{
    if (bIsInSlowMotion &&
        CanEnterSlowMotion() &&
        IsEnoughStamina(SlowMotionStaminaCost))
    {
        ExtendedStamina->ModifyStat(SlowMotionStaminaCost * -1.0f, true);
    }
    else
    {
        StopSlotMotion();
    }
}

void ABaseCharacter::Stunned()
{
    UAnimMontage* AnimMontage = GetStunMontage(ReceivedHitDirection);
    if (GameUtils::IsValid(AnimMontage))
    {
        float Duration = PlayAnimMontage(AnimMontage);
        Effects->AdjustEffectTime(EEffectType::Stun, Duration * 0.8f);
    }
    else
    {
        StateManager->ResetState(0.0f);
    }
}

void ABaseCharacter::Death()
{
    StateManager->SetState(EState::Dead);

    DynamicTargeting->DisableCameraLock();
    MeleeCollisionHandler->DeactivateCollision();
    InGameWidget->RemoveFromParent();

    GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
    GetMesh()->SetSimulatePhysics(true);

    // Enable Physics on main hand/shield items 
    if (Equipment->IsInCombat())
    {
        EItemType MainHandType = Equipment->GetSelectedMainHandType();
        ADisplayedItem* MainHandItem = Equipment->GetDisplayedItem(MainHandType, 0);

        if (GameUtils::IsValid(MainHandItem))
        {
            MainHandItem->SimulatePhysics();
        }

        if (!Equipment->IsSlotHidden(EItemType::Shield, 0))
        {
            ADisplayedItem* ShieldItem = Equipment->GetDisplayedItem(EItemType::Shield, 0);

            if (GameUtils::IsValid(ShieldItem))
            {
                ShieldItem->SimulatePhysics();
            }
        }
    }
}

void ABaseCharacter::Parried()
{
    UAnimMontage* AnimMontage = GetParriedMontage();

    if (GameUtils::IsValid(AnimMontage))
    {
        float Duration = PlayAnimMontage(AnimMontage);
        Effects->AdjustEffectTime(EEffectType::Parried, Duration * 0.8f);
    }
    else
    {
        StateManager->ResetState(0.0f);
    }
}

void ABaseCharacter::Impact()
{
    UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(EMontageAction::Impact, 0);

    if (GameUtils::IsValid(AnimMontage))
    {
        float Duration = PlayAnimMontage(AnimMontage);
        Effects->AdjustEffectTime(EEffectType::Impact, Duration * 0.8f);
    }
    else
    {
        StateManager->ResetState(0.0f);
    }
}

void ABaseCharacter::Backstabbed()
{
    UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(EMontageAction::Backstabbed, 0);

    if (GameUtils::IsValid(AnimMontage))
    {
        float Duration = PlayAnimMontage(AnimMontage);
        Effects->AdjustEffectTime(EEffectType::Backstab, Duration * 0.8f);
    }
    else
    {
        StateManager->ResetState(0.0f);
    }
}

void ABaseCharacter::SetCameraLagSmoothly(float InTargetLagSpeed)
{
    TargetLagSpeed = InTargetLagSpeed;
    GetWorld()->GetTimerManager().SetTimer(
        UpdateCameraLagTimerHandle, this, &ABaseCharacter::UpdateCameraLag, 0.016f, true);

}

void ABaseCharacter::UpdateCameraLag()
{
    CameraBoom->CameraLagSpeed =
        UKismetMathLibrary::FInterpTo_Constant(
            CameraBoom->CameraLagSpeed, TargetLagSpeed, GetWorld()->GetDeltaSeconds(), 120.0f);

    CameraBoom->CameraRotationLagSpeed = CameraBoom->CameraLagSpeed;

    if (UKismetMathLibrary::NearlyEqual_FloatFloat(CameraBoom->CameraRotationLagSpeed, TargetLagSpeed))
    {
        GetWorld()->GetTimerManager().ClearTimer(UpdateCameraLagTimerHandle);
    }

}

void ABaseCharacter::SetSprint(bool bActivate)
{
    if (bActivate)
    {
        StoredMovementState = MovementSpeed->GetMovementState();
        MovementSpeed->SetMovementState(EMovementState::Sprint);
    }
    else
    {
        if (GetWorld()->GetTimerManager().IsTimerActive(SprintLoopTimerHandle))
        {
            MovementSpeed->SetMovementState(StoredMovementState);
        }
    }
}



void ABaseCharacter::SprintLoop()
{
    if (MovementSpeed->GetMovementState() == EMovementState::Sprint &&
        IsEnoughStamina(SprintStaminaCost) &&
        !IsActivityEqual(EActivity::IsLookingForward))
    {
        if (IsIdleAndNotFalling() && GetVelocity().Size() > 10.0f)
        {
            ExtendedStamina->ModifyStat(SprintStaminaCost * -1.0f, true);
        }
    }
    else
    {
        SetSprint(false);
    }
}

bool ABaseCharacter::IsStateEqual(EState InState) const
{
    return StateManager->GetState() == InState;
}

bool ABaseCharacter::IsEnoughStamina(float InValue) const
{
    return ExtendedStamina->GetCurrentValue() >= InValue;
}

void ABaseCharacter::GetMovementVectors(FVector& InOutForward, FVector& InOutRight) const
{
    FRotator Rotator(0.0f, GetControlRotation().Yaw, 0.0f);

    InOutForward = UKismetMathLibrary::GetForwardVector(Rotator);
    InOutRight = UKismetMathLibrary::GetRightVector(Rotator);
}

bool ABaseCharacter::IsCombatTypeEqual(ECombatType InType) const
{
    return Equipment->GetCombatType() == InType;
}

bool ABaseCharacter::IsActivityEqual(EActivity InActivity) const
{
    return StateManager->GetActivityValue(InActivity);
}

bool ABaseCharacter::IsIdleAndNotFalling() const
{
    return IsStateEqual(EState::Idle) && !GetCharacterMovement()->IsFalling();
}

bool ABaseCharacter::HasMovementInput() const
{
    return UKismetMathLibrary::NotEqual_VectorVector(
        GetCharacterMovement()->GetLastInputVector(), FVector::ZeroVector, 0.0001f);
}

bool ABaseCharacter::IsCharacterAlive() const
{
    return IsAlive();
}

bool ABaseCharacter::CanUseOrSwitch() const
{
    return CanUseOrSwitchItem();
}

void ABaseCharacter::LineTraceForInteractable()
{
    float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    FVector Start = GetActorLocation();

    AActor* Con = GetController();
    FVector Forward = Con->GetActorForwardVector();
    Forward.Z = 0;

    FVector End = Start + Forward * 150.0f;

    // projectile
    ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera);
    TArray<AActor*> ActorsToIgnore;
    TArray<FHitResult> OutHits;

    if (UKismetSystemLibrary::CapsuleTraceMulti(
        GetWorld(),
        Start,
        End,
        30.0f,
        HalfHeight,
        Channel,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::Type::None,
        OutHits,
        true))
    {
        for (const FHitResult& OutHit : OutHits)
        {
            IIsInteractable* IsInteractable = Cast<IIsInteractable>(OutHit.GetActor());

            if (IsInteractable != nullptr)
            {
                IsInteractable->Interact(this);
                break;
            }
        }
    }
}

void ABaseCharacter::SetData()
{
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->NavAgentProps.AgentRadius = 42.0f;
    GetCharacterMovement()->NavAgentProps.AgentHeight = 192.0f;

    ExtendedHealth->SetStatType(EStat::Health);
    ExtendedStamina->SetStatType(EStat::Stamina);
    ExtendedStamina->SetDoesRegenerates(true);
    ExtendedStamina->SetRegenValue(1.75f);
    ExtendedStamina->SetReenableRegenTime(1.0f);

    DynamicTargeting->SetBlockingCollisionTypes(
        { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic) }
    );

    FString ObjectItemDir("/Game/DynamicCombatSystem/Blueprints/Items/ObjectItems/Instances/");
    auto SteelSwordBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelSwordBP"));
    auto SteelShieldBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelShieldBP"));
    auto HealthPotionBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("HealthPotionBP"));
    auto GreatSwordBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("GreatSwordBP"));
    auto ElvenBowBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("ElvenBowBP"));
    auto ExplosiveArrowBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("ExplosiveArrowBP"));
    auto ElvenArrowBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("ElvenArrowBP"));
    auto SummonBowBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SummonBowBP"));
    auto SummonSwordBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SummonSwordBP"));
    auto DamageBuffBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("DamageBuffBP"));
    auto HealthBuffBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("HealthBuffBP"));
    auto DrainHealthBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("DrainHealthBP"));
    auto FireballBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("FireballBP"));
    auto HealingBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("HealingBP"));
    auto InfernoBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("InfernoBP"));
    auto InstantHealBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("InstantHealBP"));
    auto LightingBeamBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("LightingBeamBP"));
    auto VortexBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("VortexBP"));
    auto TrapBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("TrapBP"));
    auto TeleportBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("TeleportBP"));
    auto MagicWandBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("MagicWandBP"));

    Inventory->SetInventory(TArray<FStoredItem>{
        FStoredItem(FGuid::NewGuid(), SteelSwordBPClass, 1),
        FStoredItem(FGuid::NewGuid(), SteelShieldBPClass, 1),
        FStoredItem(FGuid::NewGuid(), HealthPotionBPClass, 5),
        FStoredItem(FGuid::NewGuid(), GreatSwordBPClass, 1),
        FStoredItem(FGuid::NewGuid(), ElvenBowBPClass, 1),
        FStoredItem(FGuid::NewGuid(), ExplosiveArrowBPClass, 10),
        FStoredItem(FGuid::NewGuid(), ElvenArrowBPClass, 50),
        FStoredItem(FGuid::NewGuid(), SummonBowBPClass, 1),
        FStoredItem(FGuid::NewGuid(), SummonSwordBPClass, 1),
        FStoredItem(FGuid::NewGuid(), DamageBuffBPClass, 1),
        FStoredItem(FGuid::NewGuid(), HealthBuffBPClass, 1),
        FStoredItem(FGuid::NewGuid(), DrainHealthBPClass, 1),
        FStoredItem(FGuid::NewGuid(), FireballBPClass, 1),
        FStoredItem(FGuid::NewGuid(), HealingBPClass, 1),
        FStoredItem(FGuid::NewGuid(), InfernoBPClass, 1),
        FStoredItem(FGuid::NewGuid(), InstantHealBPClass, 1),
        FStoredItem(FGuid::NewGuid(), LightingBeamBPClass, 1),
        FStoredItem(FGuid::NewGuid(), VortexBPClass, 1),
        FStoredItem(FGuid::NewGuid(), TrapBPClass, 1),
        FStoredItem(FGuid::NewGuid(), TeleportBPClass, 1),
        FStoredItem(FGuid::NewGuid(), MagicWandBPClass, 1)
    });

    
    Equipment->SetEquipmentSlots({
        FEquipmentSlots(EItemType::Spell, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(FireballBPClass), 
                FStoredItem(InfernoBPClass), 
                FStoredItem(VortexBPClass), 
                FStoredItem(TeleportBPClass), 
                FStoredItem(InstantHealBPClass)
            }, 
            0, false)
        }),
        
        FEquipmentSlots(EItemType::Shield, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(SteelShieldBPClass)}, 0, false)
        }),
        
        FEquipmentSlots(EItemType::Head, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Top, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Legs, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Hands, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Feet, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Arrows, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(ElvenArrowBPClass), FStoredItem(ExplosiveArrowBPClass)
            }, 
            0, false)
        }),
        FEquipmentSlots(EItemType::Tool, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(HealthPotionBPClass), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem(), 
                FStoredItem()
            }, 
            0, false)
        }),
        FEquipmentSlots(EItemType::Ring, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false),
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false),
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false),
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::MeleeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(SteelSwordBPClass), 
                FStoredItem(GreatSwordBPClass), 
                FStoredItem()
            }, 
            0, false)
        }),
        FEquipmentSlots(EItemType::RangeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(ElvenBowBPClass), 
                FStoredItem(), 
                FStoredItem()
            }, 
            0, false)
        }),
        FEquipmentSlots(EItemType::Necklace, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
    });
    

    ExtendedMana->SetStatType(EStat::Mana);
    ExtendedMana->SetDoesRegenerates(true);
    ExtendedMana->SetRegenValue(1.75f);
    ExtendedMana->SetReenableRegenTime(1.5f);


    TargetWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 18.0f));
    TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);

    static TSubclassOf<ULockIconUI> LoadedLockIconWBClass =
        GameUtils::LoadAssetClass<ULockIconUI>("/Game/DynamicCombatSystem/Widgets/LockIconWB");

    TargetWidget->SetWidgetClass(LoadedLockIconWBClass);
    TargetWidget->SetDrawSize(FVector2D(35.0f, 35.0f));
    TargetWidget->SetHiddenInGame(true);

    ArrowSpawnLocation->SetRelativeLocation(FVector(-15.25f, 15.45f, 149.33f));
    //ArrowSpawnLocation->ComponentTags.Add(FName("ArrowSpawnLocation"));

    CameraBoom->TargetArmLength = 450.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 30.0f, 100.0f);
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->CameraLagSpeed = 15.0f;
    CameraBoom->CameraRotationLagSpeed = 15.0f;

    InitialCameraArmLength = CameraBoom->TargetArmLength;
    InitialCameraLagSpeed = CameraBoom->CameraLagSpeed;
}