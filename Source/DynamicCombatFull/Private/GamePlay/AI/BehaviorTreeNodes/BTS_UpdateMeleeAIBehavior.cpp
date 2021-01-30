// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateMeleeAIBehavior.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/AI/AICharacter.h"
#include "Components/StateManagerComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/PatrolComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Interfaces/CanBeAttacked.h"
#include "GamePlay/AI/BaseAIController.h"


UBTS_UpdateMeleeAIBehavior::UBTS_UpdateMeleeAIBehavior(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    AttackBehaviorRange(300.0f)
{
}

void UBTS_UpdateMeleeAIBehavior::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceCreated(OwnerComp);
}

void UBTS_UpdateMeleeAIBehavior::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceDestroyed(OwnerComp);

    ControlledCharacter->GetStateManager()->OnStateChanged.RemoveDynamic(
        this, &UBTS_UpdateMeleeAIBehavior::OnStateChanged);

    ControlledCharacter->GetExtendedStamina()->OnValueChanged.RemoveDynamic(
        this, &UBTS_UpdateMeleeAIBehavior::OnStaminaValueChanged);
}

void UBTS_UpdateMeleeAIBehavior::SetOwner(AActor* InActorOwner)
{
    Super::SetOwner(InActorOwner);

    AAICharacter* Character = Cast<AAICharacter>(AIOwner->GetPawn());
    if (GameUtils::IsValid(Character))
    {
        ControlledCharacter = Character;
        ControlledCharacter->GetStateManager()->OnStateChanged.AddDynamic(
            this, &UBTS_UpdateMeleeAIBehavior::OnStateChanged);

        ControlledCharacter->GetExtendedStamina()->OnValueChanged.AddDynamic(
            this, &UBTS_UpdateMeleeAIBehavior::OnStaminaValueChanged);
    }
}

void UBTS_UpdateMeleeAIBehavior::ReceiveTickAI(
    UBehaviorTreeComponent& OwnerBTree,
    AAIController* InOwnerController,
    APawn* InControlledPawn, 
    float InDeltaSeconds)
{
    Update();
}

void UBTS_UpdateMeleeAIBehavior::ReceiveSearchStartAI(
    UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn)
{
}

void UBTS_UpdateMeleeAIBehavior::OnStateChanged(EState InPrevState, EState InNewState)
{
    if (InPrevState == EState::Disabled || InNewState == EState::Disabled)
    {
        Update();
    }
}

void UBTS_UpdateMeleeAIBehavior::OnStaminaValueChanged(float InNewValue, float InMaxValue)
{
    if (InNewValue <= 0.0f)
    {
        bIsOutOfStamina = true;

        GameUtils::SetTimerRetriggerable(
            GetWorld()->GetTimerManager(),
            RetriggerableDelayTimerHandle,
            FTimerDelegate::CreateUObject(this, &UBTS_UpdateMeleeAIBehavior::Delayed_Set),
            2.0f,
            false);
    }
}

void UBTS_UpdateMeleeAIBehavior::Delayed_Set()
{
    bIsOutOfStamina = false;
}

void UBTS_UpdateMeleeAIBehavior::Update()
{
    UpdateBehavior();
    UpdateActivities();
}

void UBTS_UpdateMeleeAIBehavior::UpdateBehavior()
{
    if (AIOwner.IsValid())
    {
        if (GameUtils::IsValid(ControlledCharacter))
        {
            UStateManagerComponent* StateManager = ControlledCharacter->GetStateManager();
            UStatsManagerComponent* StatsManager = ControlledCharacter->GetStatsManager();
            UEquipmentComponent* Equipment = ControlledCharacter->GetEquipment();

            if (StateManager->GetState() == EState::Disabled)
            {
                SetBehavior(EAIBehavior::Hit);
            }
            else
            {
                UBlackboardComponent* BlackboardComp = AIOwner->GetBlackboardComponent();
                if (!GameUtils::IsValid(BlackboardComp))
                {
                    return;
                }

                AActor* Target =
                    Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

                EAIBehavior Behavior = ControlledCharacter->GetPatrol()->IsPatrolPathValid() ?
                    EAIBehavior::Patrol : EAIBehavior::Idle;

                if (Target != nullptr)
                {
                    ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(Target);
                    if (CanBeAttacked != nullptr)
                    {
                        if (CanBeAttacked->IsAlive())
                        {
                            float DistanceToTarget = Target->GetDistanceTo(ControlledCharacter);
                            float DotProductToTarget = Target->GetDotProductTo(ControlledCharacter);
                            int ReceivedHitsCount = StatsManager->GetRecentlyReceivedHitsCount();
                            float Value = ControlledCharacter->GetExtendedStamina()->GetCurrentValue();
                            float MaxValue = ControlledCharacter->GetExtendedStamina()->GetMaxValue();
                            float StaminaPercent = (Value / MaxValue) * 100.0f;

                            UEquipmentComponent* TargetEquipment =
                                Cast<UEquipmentComponent>(Target->GetComponentByClass(UEquipmentComponent::StaticClass()));

                            if (DotProductToTarget <= -0.25f)
                            {
                                if (DistanceToTarget <= AttackBehaviorRange)
                                {
                                    SetBehavior(EAIBehavior::MeleeAttack);
                                }
                                else
                                {
                                    SetBehavior(EAIBehavior::Approach);
                                }
                            }
                            else
                            {
                                if (DistanceToTarget <= AttackBehaviorRange)
                                {
                                    if (TicksNearTarget >= 3.0f)
                                    {
                                        SetBehavior(EAIBehavior::MeleeAttack);
                                        if (UKismetMathLibrary::RandomBoolWithWeight(0.4f))
                                        {
                                            TicksNearTarget = 0.0f;
                                        }
                                    }
                                    else
                                    {
                                        TicksNearTarget = TicksNearTarget + 1.0f;

                                        if (bIsOutOfStamina || 
                                            StaminaPercent <= 40.0f || 
                                            UKismetMathLibrary::RandomBoolWithWeight(0.1f))
                                        {
                                            SetBehavior(EAIBehavior::MeleeAttack);
                                        }
                                        else
                                        {
                                            if (GameUtils::IsValid(TargetEquipment))
                                            {
                                                TArray<ECombatType> Array { ECombatType::Magic, ECombatType::Range };
                                                
                                                if (Array.Contains(TargetEquipment->GetCombatType()))
                                                {
                                                    SetBehavior(EAIBehavior::MeleeAttack);
                                                }
                                                else
                                                {
                                                    SetBehavior(EAIBehavior::StrafeAround);
                                                }
                                            }
                                            else
                                            {
                                                SetBehavior(EAIBehavior::StrafeAround);
                                            }

                                        }
                                    }
                                }
                                else
                                {
                                    TicksNearTarget = 0.0f;

                                    if (DistanceToTarget >= 2000.0f)
                                    {
                                        SetBehavior(EAIBehavior::Approach);
                                    }
                                    else
                                    {
                                        if (GameUtils::IsValid(TargetEquipment))
                                        {
                                            if (TargetEquipment->IsInCombat())
                                            {
                                                TArray<ECombatType> Array { ECombatType::Magic, ECombatType::Range };
                                                if (Array.Contains(TargetEquipment->GetCombatType()))
                                                {
                                                    SetBehavior(EAIBehavior::Approach);
                                                }
                                                else
                                                {
                                                    SetBehavior(EAIBehavior::StrafeAround);
                                                }
                                            }
                                            else
                                            {
                                                SetBehavior(EAIBehavior::Approach);
                                            }
                                        }
                                        else
                                        {
                                            SetBehavior(EAIBehavior::Approach);
                                        }

                                    }
                                }
                            }
                        }
                        else
                        {
                            SetBehavior(Behavior);
                        }
                    }
                }
                else
                {
                    SetBehavior(Behavior);
                }

            }
        }
    }
}

void UBTS_UpdateMeleeAIBehavior::UpdateActivities()
{
    if (AIOwner.IsValid())
    {
        if (GameUtils::IsValid(ControlledCharacter))
        {
            UBlackboardComponent* BlackboardComp = AIOwner->GetBlackboardComponent();
            if (!GameUtils::IsValid(BlackboardComp))
            {
                return;
            }

            EAIBehavior AIBehaviorType =
                (EAIBehavior)BlackboardComp->GetValueAsEnum(BehaviorKey.SelectedKeyName);

            UStateManagerComponent* StateManager = ControlledCharacter->GetStateManager();
            UEquipmentComponent* Equipment = ControlledCharacter->GetEquipment();

            if (AIBehaviorType == EAIBehavior::Approach || 
                AIBehaviorType == EAIBehavior::StrafeAround)
            {
                bool bValue = !bIsOutOfStamina && Equipment->IsInCombat();
                StateManager->SetActivity(EActivity::IsBlockingPressed, bValue);
            }
            else
            {
                StateManager->SetActivity(EActivity::IsBlockingPressed, false);
            }
        }
    }
}

void UBTS_UpdateMeleeAIBehavior::SetBehavior(EAIBehavior InBehavior)
{
    UBlackboardComponent* BlackboardComp = AIOwner->GetBlackboardComponent();
    if (GameUtils::IsValid(BlackboardComp))
    {
        BlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, (uint8)InBehavior);
    }
}

