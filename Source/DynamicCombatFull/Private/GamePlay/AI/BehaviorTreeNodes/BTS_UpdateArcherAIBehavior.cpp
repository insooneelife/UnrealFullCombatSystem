// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateArcherAIBehavior.h"
#include "BehaviorTree/BTFunctionLibrary.h"
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

UBTS_UpdateArcherAIBehavior::UBTS_UpdateArcherAIBehavior(const FObjectInitializer& ObjectInitializer)
    : 
    Super(ObjectInitializer),
    FleeBehaviorRange(300.0f)
{
}

void UBTS_UpdateArcherAIBehavior::ReceiveTickAI(
    UBehaviorTreeComponent& OwnerBTree,
    AAIController* InOwnerController, 
    APawn* InControlledPawn,
    float InDeltaSeconds)
{
    Update();
}

void UBTS_UpdateArcherAIBehavior::ReceiveSearchStartAI(
    UBehaviorTreeComponent& OwnerBTree,
    AAIController* InOwnerController,
    APawn* InControlledPawn)
{
    AAICharacter* Character = Cast<AAICharacter>(InControlledPawn);
    if (GameUtils::IsValid(Character))
    {
        ControlledCharacter = Character;
        OwnerController = InOwnerController;

        Character->GetStateManager()->OnStateChanged.AddDynamic(this, &UBTS_UpdateArcherAIBehavior::OnStateChanged);
    }
}

void UBTS_UpdateArcherAIBehavior::OnStateChanged(EState InPrevState, EState InNewState)
{
    if ((InPrevState == EState::Disabled) ||
        (InNewState == EState::Disabled) ||
        (InNewState == EState::Attacking))
    {
        Update();
    }
}

void UBTS_UpdateArcherAIBehavior::Update()
{
    UpdateBehavior();
    UpdateActivities();
}

void UBTS_UpdateArcherAIBehavior::Flee()
{
    ABaseAIController* Controller = Cast<ABaseAIController>(OwnerController);
    Controller->UpdateSenseTarget();
    bIsFleeing = true;

    GameUtils::SetTimerRetriggerable(
        GetWorld()->GetTimerManager(),
        RetriggerableDelayTimerHandle,
        FTimerDelegate::CreateUObject(this, &UBTS_UpdateArcherAIBehavior::Delayed_Flee),
        3.0f,
        false);
}

void UBTS_UpdateArcherAIBehavior::Delayed_Flee()
{
    bIsFleeing = false;
}

void UBTS_UpdateArcherAIBehavior::UpdateBehavior()
{
    if (GameUtils::IsValid(OwnerController))
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
                UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(this);
                if (!GameUtils::IsValid(BlackboardComp))
                {
                    return;
                }

                AActor* Target = 
                    Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

                EAIBehavior Behavior = ControlledCharacter->GetPatrol()->IsPatrolPathValid() ?
                    EAIBehavior::Patrol : EAIBehavior::Idle;

                if (GameUtils::IsValid(Target))
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

                            if (bIsFleeing)
                            {
                                SetBehavior(EAIBehavior::Flee);
                            }
                            else
                            {
                                if (DistanceToTarget <= FleeBehaviorRange)
                                {
                                    TArray<ECombatType> Array{ ECombatType::Magic, ECombatType::Range };
                                    bool bContains = Array.Contains(TargetEquipment->GetCombatType());

                                    if (bContains)
                                    {
                                        SetBehavior(EAIBehavior::RangeAttack);
                                    }
                                    else
                                    {
                                        SetBehavior(EAIBehavior::Flee);
                                        Flee();
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

void UBTS_UpdateArcherAIBehavior::UpdateActivities()
{
    if (GameUtils::IsValid(OwnerController))
    {
        if (GameUtils::IsValid(ControlledCharacter))
        {
            UBlackboardComponent* BlackboardComp = UBTFunctionLibrary::GetOwnersBlackboard(this);

            if (!GameUtils::IsValid(BlackboardComp))
            {
                return;
            }

            EAIBehavior AIBehaviorType =
                (EAIBehavior)BlackboardComp->GetValueAsEnum(BehaviorKey.SelectedKeyName);

            UStateManagerComponent* StateManager = ControlledCharacter->GetStateManager();
            UEquipmentComponent* Equipment = ControlledCharacter->GetEquipment();

            if (AIBehaviorType == EAIBehavior::RangeAttack)
            {
                bool bValue = StateManager->GetState() == EState::Idle && Equipment->IsInCombat();
                StateManager->SetActivity(EActivity::IsAimingPressed, bValue);
            }
            else
            {
                StateManager->SetActivity(EActivity::IsAimingPressed, false);
            }
        }
    }    
}

void UBTS_UpdateArcherAIBehavior::SetBehavior(EAIBehavior InBehavior)
{
    UBTFunctionLibrary::SetBlackboardValueAsEnum(this, BehaviorKey, (uint8)InBehavior);
}
