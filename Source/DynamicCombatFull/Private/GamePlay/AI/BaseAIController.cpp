// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Components/EquipmentComponent.h"
#include "Components/BehaviorComponent.h"
#include "GameCore/DefaultGameInstance.h"
#include "AICharacter.h"

ABaseAIController::ABaseAIController()
    :
    TargetKey("Target"),
    AttackTypeKey("AttackType"),
    StateKey("State"),
    IsInCombatKey("IsInCombat")
{
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    PossesedAICharacter = Cast<AAICharacter>(InPawn);

    if (PossesedAICharacter->IsValidLowLevel())
    {
        RunBehaviorTree(PossesedAICharacter->GetBTree());
        FTimerHandle UpdateTargetTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            UpdateTargetTimerHandle, this, &ABaseAIController::UpdateTarget, 1.0f, true);
        PossesedAICharacter->GetEquipment()->OnInCombatChanged.AddDynamic(this, &ABaseAIController::OnInCombatChanged);
    }
}

void ABaseAIController::UpdateTarget()
{
    TArray<AActor*> LocalPerceivedActors;
    TArray<AActor*> LocalEnemyActors;

    ICanBeAttacked* CanBeAttackedOwner = Cast<ICanBeAttacked>(GetPawn());
    if (CanBeAttackedOwner != nullptr)
    {
        if (CanBeAttackedOwner->IsAlive())
        {
            TArray<AActor*> OutActors;
            AIPerception->GetKnownPerceivedActors(nullptr, OutActors);

            for (AActor* Actor : OutActors)
            {
                ICanBeAttacked* CanBeAttackedActor = Cast<ICanBeAttacked>(Actor);

                if (CanBeAttackedActor != nullptr)
                {
                    if (CanBeAttackedActor->IsAlive())
                    {
                        LocalPerceivedActors.Add(Actor);
                    }
                }
            }

            LocalEnemyActors = UDefaultGameInstance::SelectEnemyActors(GetPawn(), LocalPerceivedActors);

            if (LocalEnemyActors.Num() > 0)
            {
                AActor* TargetActor = UDefaultGameInstance::GetClosestActor(GetPawn(), LocalEnemyActors);
                SetTarget(TargetActor);
            }
            else
            {
                SetTarget(nullptr);
            }
            
            ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            if (LocalPerceivedActors.Contains(PlayerCharacter))
            {
                PossesedAICharacter->ShowStatsWidget();
            }
            else
            {
                PossesedAICharacter->HideStatsWidget();
            }
        }
    }
}

void ABaseAIController::SetTarget(AActor* InNewTarget)
{
    if (InNewTarget != Target)
    {
        Target = InNewTarget;

        if (Blackboard->IsValidLowLevel())
        {
            Blackboard->SetValueAsObject(TargetKey, InNewTarget);
        }
    }
}

void ABaseAIController::SetIsInCombat(bool bValue)
{
    if (bValue != bIsInCombat)
    {
        bIsInCombat = bValue;
        if (Blackboard->IsValidLowLevel())
        {
            Blackboard->SetValueAsBool(IsInCombatKey, bIsInCombat);
        }
    }
}

void ABaseAIController::UpdateSenseTarget()
{
    // Update target with sense to keep it in perceived actors array
    // e.g if AI is running away from the target, when he is turned back to it for some time, 
    // it will lose it, to prevent it this method can be called to keep it up

    FVector ActorLocation = GetPawn()->GetActorLocation();
    UAISense_Damage::ReportDamageEvent(GetWorld(), GetPawn(), Target, 1.0f, ActorLocation, ActorLocation);
}

bool ABaseAIController::IsEnemy(const FAIStimulus& InAIStimulus, AActor* InActor) const
{
    ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(InActor);
    if (CanBeAttacked != nullptr)
    {
        UBehaviorComponent* BehaviorComp = 
            Cast<UBehaviorComponent>(GetPawn()->GetComponentByClass(UBehaviorComponent::StaticClass()));

        if (!BehaviorComp->IsValidLowLevel())
        {
            return false;
        }

        if (CanBeAttacked->IsAlive() && 
            BehaviorComp->IsEnemy(InActor) && 
            InAIStimulus.WasSuccessfullySensed())
        {
            return true;
        }
    }

    return false;
}

void ABaseAIController::OnInCombatChanged(bool bInIsInCombat)
{
    SetIsInCombat(bInIsInCombat);
}
