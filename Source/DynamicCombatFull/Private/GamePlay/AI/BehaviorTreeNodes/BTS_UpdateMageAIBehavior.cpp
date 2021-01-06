// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateMageAIBehavior.h"
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

UBTS_UpdateMageAIBehavior::UBTS_UpdateMageAIBehavior(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    MeleeAttackBehaviorRange(1000.0f)
{
}

void UBTS_UpdateMageAIBehavior::ReceiveTickAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn, float InDeltaSeconds)
{
    Update();
}

void UBTS_UpdateMageAIBehavior::ReceiveSearchStartAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn)
{
    AAICharacter* Character = Cast<AAICharacter>(InControlledPawn);
    if (GameUtils::IsValid(Character))
    {
        ControlledCharacter = Character;
        OwnerController = InOwnerController;

        Character->GetStateManager()->OnStateChanged.AddDynamic(
            this, &UBTS_UpdateMageAIBehavior::OnStateChanged);
    }
}

void UBTS_UpdateMageAIBehavior::OnStateChanged(EState InPrevState, EState InNewState)
{
    if (InPrevState == EState::Disabled ||
        InNewState == EState::Disabled ||
        InNewState == EState::Attacking)
    {
        Update();
    }
}

void UBTS_UpdateMageAIBehavior::Update()
{
    UpdateBehavior();
}

void UBTS_UpdateMageAIBehavior::UpdateBehavior()
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
                            
                            if (DistanceToTarget <= MeleeAttackBehaviorRange)
                            {
                                SetBehavior(EAIBehavior::MeleeAttack);
                            }
                            else
                            {
                                SetBehavior(EAIBehavior::RangeAttack);
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

void UBTS_UpdateMageAIBehavior::SetBehavior(EAIBehavior InBehavior)
{
    UBTFunctionLibrary::SetBlackboardValueAsEnum(this, BehaviorKey, (uint8)InBehavior);
}
