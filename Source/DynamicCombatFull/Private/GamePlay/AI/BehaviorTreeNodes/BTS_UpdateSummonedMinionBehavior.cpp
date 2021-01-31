// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateSummonedMinionBehavior.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/AI/AICharacter.h"
#include "Interfaces/CanBeAttacked.h"
#include "Components/PatrolComponent.h"
#include "Components/StateManagerComponent.h"

UBTS_UpdateSummonedMinionBehavior::UBTS_UpdateSummonedMinionBehavior(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    AttackBehaviorRange(500.0f)
{
}

void UBTS_UpdateSummonedMinionBehavior::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceCreated(OwnerComp);
}

void UBTS_UpdateSummonedMinionBehavior::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
    Super::OnInstanceDestroyed(OwnerComp);

    if (ControlledCharacter.IsValid())
    {
        ControlledCharacter->GetStateManager()->OnStateChanged.RemoveDynamic(
            this, &UBTS_UpdateSummonedMinionBehavior::OnStateChanged);
    }
}

void UBTS_UpdateSummonedMinionBehavior::SetOwner(AActor* InActorOwner)
{
    Super::SetOwner(InActorOwner);

    AAICharacter* Character = Cast<AAICharacter>(AIOwner->GetPawn());
    if (GameUtils::IsValid(Character))
    {
        ControlledCharacter = Character;
        ControlledCharacter->GetStateManager()->OnStateChanged.AddDynamic(
            this, &UBTS_UpdateSummonedMinionBehavior::OnStateChanged);
    }
}

void UBTS_UpdateSummonedMinionBehavior::ReceiveTickAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn, float InDeltaSeconds)
{
    UpdateBehavior();
}

void UBTS_UpdateSummonedMinionBehavior::ReceiveSearchStartAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn)
{
}

void UBTS_UpdateSummonedMinionBehavior::OnStateChanged(EState InPrevState, EState InNewState)
{
    if (InPrevState == EState::Disabled || InNewState == EState::Disabled)
    {
        UpdateBehavior();
    }
}

void UBTS_UpdateSummonedMinionBehavior::UpdateBehavior()
{
    if (AIOwner.IsValid())
    {
        if (ControlledCharacter.IsValid())
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
                            float DistanceToTarget = Target->GetDistanceTo(ControlledCharacter.Get());

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

void UBTS_UpdateSummonedMinionBehavior::SetBehavior(EAIBehavior InBehavior)
{
    UBlackboardComponent* BlackboardComp = AIOwner->GetBlackboardComponent();
    if (GameUtils::IsValid(BlackboardComp))
    {
        BlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, (uint8)InBehavior);
    }
}


