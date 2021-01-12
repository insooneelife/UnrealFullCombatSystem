// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "BrainComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Damage.h"

#include "GamePlay/AI/BaseAIController.h"
#include "GamePlay/PatrolPathActor.h"
#include "GameCore/GameUtils.h"
#include "GameCore/CustomStructs.h"
#include "GameCore/DefaultGameInstance.h"
#include "Components/StatsManagerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/EffectsComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/DissolveComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/BehaviorComponent.h"
#include "Components/MovementSpeedComponent.h"
#include "Components/PatrolComponent.h"
#include "Components/RotatingComponent.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "UI/AIStatBarsUI.h"
#include "UI/StatBarUI.h"
#include "UI/LockIconUI.h"
#include "GamePlay/BaseCharacter.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    ReceivedHitDirection = EDirection::Front;
    RecentlyReceivedDamageStunLimit = 40.0f;
    RecentlyReceivedHitsCountStunLimit = 2;

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



    static TSubclassOf<ABaseAIController> LoadedClass =
        GameUtils::LoadAssetClass<ABaseAIController>("/Game/DynamicCombatSystem/Blueprints/AI/BaseAIControllerBP");
    AIControllerClass = LoadedClass;

    MovementSpeed = CreateDefaultSubobject<UMovementSpeedComponent>("MovementSpeed");
    Patrol = CreateDefaultSubobject<UPatrolComponent>("Patrol");
    StateManager = CreateDefaultSubobject<UStateManagerComponent>("StateManager");
    StatsManager = CreateDefaultSubobject<UStatsManagerComponent>("StatsManager");
    Rotating = CreateDefaultSubobject<URotatingComponent>("Rotating");
    ExtendedStamina = CreateDefaultSubobject<UExtendedStatComponent>("ExtendedStamina");
    Equipment = CreateDefaultSubobject<UEquipmentComponent>("Equipment");
    Dissolve = CreateDefaultSubobject<UDissolveComponent>("Dissolve");
    Effects = CreateDefaultSubobject<UEffectsComponent>("Effects");
    Behavior = CreateDefaultSubobject<UBehaviorComponent>("Behavior");
    MontageManager = CreateDefaultSubobject<UMontageManagerComponent>("MontageManager");
    ExtendedHealth = CreateDefaultSubobject<UExtendedStatComponent>("ExtendedHealth");
    MeleeCollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>("MeleeCollisionHandler");

    TargetWidget = CreateDefaultSubobject<UWidgetComponent>("TargetWidget");
    TargetWidget->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

    StatBarsWidget = CreateDefaultSubobject<UWidgetComponent>("StatBarsWidget");
    StatBarsWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

    SetData();
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

    StatsManager->Init();
    Equipment->Init();
    Patrol->Init();
    InitializeStatsWidget();

    ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetController());

    if (GameUtils::IsValid(BaseAIController))
    {
        AIController = BaseAIController;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is not valid!"));
    }


    float Health = ExtendedHealth->GetMaxValue();
    ExtendedHealth->SetCurrentValue(Health, false);

    Effects->OnEffectApplied.AddDynamic(this, &AAICharacter::OnEffectApplied);
    Effects->OnEffectRemoved.AddDynamic(this, &AAICharacter::OnEffectRemoved);
    MeleeCollisionHandler->OnHit.AddDynamic(this, &AAICharacter::OnHit);
    MeleeCollisionHandler->OnCollisionActivated.AddDynamic(this, &AAICharacter::OnCollisionActivated);
    ExtendedHealth->OnValueChanged.AddDynamic(this, &AAICharacter::OnValueChanged_ExtendedHealth);
}

void AAICharacter::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    Effects->OnEffectApplied.RemoveDynamic(this, &AAICharacter::OnEffectApplied);
    Effects->OnEffectRemoved.RemoveDynamic(this, &AAICharacter::OnEffectRemoved);
    MeleeCollisionHandler->OnHit.RemoveDynamic(this, &AAICharacter::OnHit);
    MeleeCollisionHandler->OnCollisionActivated.RemoveDynamic(this, &AAICharacter::OnCollisionActivated);
    ExtendedHealth->OnValueChanged.RemoveDynamic(this, &AAICharacter::OnValueChanged_ExtendedHealth);

    Super::EndPlay(EndPlayResult);
}

void AAICharacter::OnConstruction(const FTransform& Transform)
{    
     
}

EState AAICharacter::GetState() const
{
    return StateManager->GetState();
}

void AAICharacter::OnEffectApplied(EEffectType InType)
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

}

void AAICharacter::OnEffectRemoved(EEffectType InType)
{
    if (InType == EEffectType::Stun ||
        InType == EEffectType::Knockdown ||
        InType == EEffectType::Impact ||
        InType == EEffectType::Parried ||
        InType == EEffectType::Backstab)
    {
        TArray<EEffectType> Array 
        {
            EEffectType::Stun,
            EEffectType::Knockdown,
            EEffectType::Impact,
            EEffectType::Parried,
            EEffectType::Backstab
        };

        if (!Effects->IsAnyEffectApplied(Array))
        {
            StateManager->ResetState(0.0f);
        }
    }
}

void AAICharacter::HandleMeshOnDeath()
{
    GetAttachedActors(AttachedActors);

    GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
    GetMesh()->SetSimulatePhysics(true);

    // Simulate physics on main hand/shield items if character is in combat
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

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, this, &AAICharacter::Delayed_HandleMeshOnDeath, 3.0f, false);
}

void AAICharacter::Delayed_HandleMeshOnDeath()
{
    for (AActor* Actor : AttachedActors)
    {
        if (GameUtils::IsValid(Actor))
        {
            TArray<UActorComponent*> DissolveComponents =
                Actor->GetComponentsByTag(UPrimitiveComponent::StaticClass(), "Dissolve");

            for (UActorComponent* DissolveComp : DissolveComponents)
            {
                Dissolve->StartDissolve(Cast<UPrimitiveComponent>(DissolveComp), false);

            }
        }
    }

    Dissolve->StartDissolve(GetMesh(), false);
}

void AAICharacter::OnValueChanged_ExtendedHealth(float InNewValue, float InMaxValue)
{
    if (InNewValue <= 0.0f)
    {
        Death();
    }
}

void AAICharacter::InitializeStatsWidget()
{
    UAIStatBarsUI* AIStatBarsUI = Cast<UAIStatBarsUI>(StatBarsWidget->GetUserWidgetObject());

    if (GameUtils::IsValid(AIStatBarsUI))
    {
        AIStatBarsUI->GetHealth()->Init(ExtendedHealth);
        AIStatBarsUI->GetStamina()->Init(ExtendedStamina);
    }
}

void AAICharacter::ShowStatsWidget()
{
    StatBarsWidget->SetHiddenInGame(false);
}

void AAICharacter::HideStatsWidget()
{
    StatBarsWidget->SetHiddenInGame(true);
}

void AAICharacter::Death()
{
    StateManager->SetState(EState::Dead);
    HandleMeshOnDeath();

    UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
    if (GameUtils::IsValid(AnimInstance))
    {
        AnimInstance->Montage_Stop(0.2f);
    }

    AIController->StopMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    if (GameUtils::IsValid(AIController->GetBrainComponent()))
    {
        AIController->GetBrainComponent()->StopLogic("");
        HideStatsWidget();

        MeleeCollisionHandler->DeactivateCollision();

        SetLifeSpan(8.0f);
    }    
}

void AAICharacter::Stunned()
{
    PlayEffectMontage(GetStunMontage(ReceivedHitDirection), EEffectType::Stun);
}

void AAICharacter::Parried()
{
    PlayEffectMontage(GetParriedMontage(), EEffectType::Parried);
}

void AAICharacter::Impact()
{
    PlayEffectMontage(GetImpactMontage(), EEffectType::Impact);
}

void AAICharacter::Backstabbed()
{
    UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(EMontageAction::Backstabbed, 0);
    PlayEffectMontage(AnimMontage, EEffectType::Backstab);
}

void AAICharacter::PlayEffectMontage(UAnimMontage* AnimMontage, EEffectType EffectType)
{
    AIController->StopMovement();

    UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
    if (GameUtils::IsValid(AnimInstance))
    {
        AnimInstance->Montage_Stop(0.2f);

        if (GameUtils::IsValid(AnimMontage))
        {
            float Duration = PlayAnimMontage(AnimMontage);
            Effects->AdjustEffectTime(EffectType, Duration);
        }
    }
}

float AAICharacter::Roll(EDirection InDirection)
{
    StateManager->SetState(EState::Rolling);
    UAnimMontage* AnimMontage = GetRollMontage(InDirection);

    if (GameUtils::IsValid(AnimMontage))
    {
        float Duration = PlayAnimMontage(AnimMontage);

        StateManager->ResetState(Duration);
        return Duration;
    }
    else
    {
        StateManager->ResetState(0.0f);
        return 0.0f;
    }
}

bool AAICharacter::IsStateEqualPure(EState InState) const
{
    return (StateManager->GetState() == InState);
}

bool AAICharacter::IsActivityPure(EActivity InActivity) const
{
    return StateManager->GetActivityValue(InActivity);
}

bool AAICharacter::IsCombatTypePure(ECombatType InType) const
{
    return Equipment->GetCombatType() == InType;
}

void AAICharacter::OnSelected()
{
    TargetWidget->SetHiddenInGame(true);
}

void AAICharacter::OnDeselected()
{
    TargetWidget->SetHiddenInGame(false);
}

bool AAICharacter::IsTargetable() const
{
    return IsAlive();
}

void AAICharacter::ReportDamage(const FHitData& InHitData)
{
    UAISense_Damage::ReportDamageEvent(
        GetWorld(), 
        this,
        InHitData.DamageCauser, 
        InHitData.Damage,
        InHitData.DamageCauser->GetActorLocation(),
        GetActorLocation());
}

bool AAICharacter::IsAlive() const
{
    return !IsStateEqualPure(EState::Dead);
}

FName AAICharacter::GetHeadSocket() const
{
    return FName("head");
}

bool AAICharacter::CanBeAttacked() const
{
    return IsAlive() && !IsActivityPure(EActivity::IsImmortal);
}

void AAICharacter::Block()
{
    UAnimMontage* AnimMontage = GetBlockMontage();

    if (GameUtils::IsValid(AnimMontage))
    {
        PlayAnimMontage(AnimMontage);
    }
}

bool AAICharacter::IsBlocked() const
{
    return
        ReceivedHitDirection == EDirection::Front &&
        IsActivityPure(EActivity::IsBlockingPressed);
}

void AAICharacter::UpdateReceivedHitDirection(FVector InHitFromDirection)
{
    ReceivedHitDirection = UDefaultGameInstance::GetHitDirection(InHitFromDirection, this);
}

bool AAICharacter::CanBeStunned() const
{
    if (Effects->IsEffectApplied(EEffectType::Backstab))
    {
        return false;
    }
    else
    {
        if (StatsManager->GetRecentlyReceivedSuccessfulDamage() < RecentlyReceivedDamageStunLimit)
        {
            return true;
        }
        else
        {
            return StatsManager->GetRecentlyReceivedSuccessfulHitsCount() <= RecentlyReceivedHitsCountStunLimit;
        }
    }
}

bool AAICharacter::CanBeInterrupted() const
{
    return !IsActivityPure(EActivity::CantBeInterrupted);
}

bool AAICharacter::CanBeBackstabbed() const
{
    return 
        !Effects->IsEffectApplied(EEffectType::Backstab) &&
        !GameUtils::IsValid(AIController->GetTarget());
}

FRotator AAICharacter::GetDesiredRotation() const
{
    UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();

    if (GameUtils::IsValid(Blackboard))
    {
        if (GameUtils::IsValid(AIController))
        {
            AActor* Target = AIController->GetTarget();
            if (Target != nullptr)
            {
                FVector ActorLoc = GetActorLocation();
                FVector TargetActorLoc =  Target->GetActorLocation();

                FRotator ActorRot = GetActorRotation();
                FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(ActorLoc, TargetActorLoc);

                return FRotator(ActorRot.Pitch, LookAtRot.Yaw, ActorRot.Roll);
            }
        }
    }

    return GetActorRotation();
}

UDataTable* AAICharacter::GetMontages(EMontageAction InAction) const
{
    return nullptr;
}


float AAICharacter::GetMeleeDamage() const
{
    return StatsManager->GetDamage();
}

float AAICharacter::MeleeAttack(EMeleeAttackType InType)
{
    MeleeAttackType = InType;
    StateManager->SetState(EState::Attacking);
    GetWorld()->GetTimerManager().ClearTimer(ResetMeleeAttackCounterTimerHandle);

    UAnimMontage* AnimMontage = GetNextMeleeAttackMontage(MontageManager, MeleeAttackType);

    if (GameUtils::IsValid(AnimMontage))
    {
        float Value = StatsManager->GetStatValue(EStat::AttackSpeed, true);
        float Duration = PlayAnimMontage(AnimMontage, Value);
        float Time = Duration * 1.5f;

        StateManager->ResetState(Duration);

        GetWorld()->GetTimerManager().SetTimer(
            ResetMeleeAttackCounterTimerHandle, this, &AAICharacter::ResetMeleeAttackCounter, Time, false);

        float StaminaCost = StatsManager->GetStatValue(EStat::MeleeAttackStaminaCost, true);
        float FinalCost = UDefaultGameInstance::ScaleMeleeAttackStaminaCostByType(StaminaCost, MeleeAttackType);
        ExtendedStamina->ModifyStat(FinalCost * -1.0f, true);

        return Duration;
    }
    else
    {
        StateManager->ResetState(0.0f);
    }
    
    return 0.0f;
}

UAnimMontage* AAICharacter::GetRollMontage(EDirection InDirection) const
{
    EMontageAction MontageActionType;

    if (InDirection == EDirection::Front)
    {
        MontageActionType = EMontageAction::RollForward;
    }
    else if (InDirection == EDirection::Back)
    {
        MontageActionType = EMontageAction::RollBackward;
    }
    else if (InDirection == EDirection::Right)
    {
        MontageActionType = EMontageAction::RollRight;
    }
    else
    {
        MontageActionType = EMontageAction::RollLeft;
    }

    return MontageManager->GetMontageForAction(MontageActionType, 0);
}


void AAICharacter::SetData()
{
    GetCharacterMovement()->bOrientRotationToMovement = true;

    GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
    GetCapsuleComponent()->SetCapsuleRadius(42.0f);

    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
    GetMesh()->SetRelativeRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f)));

    MeleeCollisionHandler->AddIgnoredClass(AAICharacter::StaticClass());

    ExtendedHealth->SetStatType(EStat::Health);
    ExtendedStamina->SetStatType(EStat::Stamina);
    ExtendedStamina->SetDoesRegenerates(true);
    ExtendedStamina->SetRegenValue(2.0f);
    ExtendedStamina->SetReenableRegenTime(1.5f);

    Behavior->AddEnemy(ABaseCharacter::StaticClass());

    Dissolve->SetDissolveInterpSpeed(0.4f);

    MovementSpeed->SetWalkSpeed(185.0f);
    MovementSpeed->SetJogSpeed(375.0f);
    MovementSpeed->SetSprintSpeed(500.0f);


    TargetWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 18.0f));
    TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);

    static TSubclassOf<ULockIconUI> LoadedLockIconWBClass =
        GameUtils::LoadAssetClass<ULockIconUI>("/Game/DynamicCombatSystem/Widgets/LockIconWB");

    TargetWidget->SetWidgetClass(LoadedLockIconWBClass);
    TargetWidget->SetDrawSize(FVector2D(150.0f, 10.0f));
    TargetWidget->SetHiddenInGame(true);


    StatBarsWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
    StatBarsWidget->SetWidgetSpace(EWidgetSpace::Screen);

    static TSubclassOf<UAIStatBarsUI> LoadedAIStatBarsUIClass =
        GameUtils::LoadAssetClass<UAIStatBarsUI>("/Game/DynamicCombatSystem/Widgets/AIStatBarsWB");

    StatBarsWidget->SetWidgetClass(LoadedAIStatBarsUIClass);
    StatBarsWidget->SetDrawSize(FVector2D(150.0f, 10.0f));
    StatBarsWidget->SetHiddenInGame(true);

}
