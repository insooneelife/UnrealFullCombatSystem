// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Strafe.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameCore/GameUtils.h"
#include "Components/RotatingComponent.h"

UBTS_Strafe::UBTS_Strafe(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static UEnvQuery* EQS_StrafeLeft =
        GameUtils::LoadAssetObject<UEnvQuery>("/Game/DynamicCombatSystem/Blueprints/AI/EQS/EQS_StrafeLeft2");

    EQS_StrafeLeftObject = EQS_StrafeLeft;

    static UEnvQuery* EQS_StrafeRight =
        GameUtils::LoadAssetObject<UEnvQuery>("/Game/DynamicCombatSystem/Blueprints/AI/EQS/EQS_StrafeRight2");

    EQS_StrafeRightObject = EQS_StrafeRight;
}

void UBTS_Strafe::ReceiveTickAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn, float InDeltaSeconds)
{
    Strafe(OwnerBTree);
}

void UBTS_Strafe::ReceiveActivationAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn)
{
    ControlledPawn = InControlledPawn;
    OwnerController = InOwnerController;
    
    AActor* Target = Cast<AActor>(OwnerBTree.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
    OwnerController->SetFocus(Target, EAIFocusPriority::Gameplay);

    URotatingComponent* Rotating =
        Cast<URotatingComponent>(ControlledPawn->GetComponentByClass(URotatingComponent::StaticClass()));

    if (GameUtils::IsValid(Rotating))
    {
        Rotating->SetRotationMode(ERotationMode::FaceCamera);
        Strafe(OwnerBTree);
    }
}

void UBTS_Strafe::ReceiveDeactivationAI(UBehaviorTreeComponent& OwnerBTree, AAIController* InOwnerController, APawn* InControlledPawn)
{
    OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
    OwnerController->StopMovement();

    URotatingComponent* Rotating =
        Cast<URotatingComponent>(ControlledPawn->GetComponentByClass(URotatingComponent::StaticClass()));

    if (GameUtils::IsValid(Rotating))
    {
        Rotating->SetRotationMode(ERotationMode::OrientToMovement);
    }
}

void UBTS_Strafe::Strafe(const UBehaviorTreeComponent& OwnerBTree)
{
    UEnvQuery* Query = GetStrafeQuery(OwnerBTree);
    UEnvQueryInstanceBlueprintWrapper* QueryInstance =
        UEnvQueryManager::RunEQSQuery(GetWorld(), Query, ControlledPawn, EEnvQueryRunMode::RandomBest5Pct, nullptr);

    QueryInstance->GetOnQueryFinishedEvent().AddUniqueDynamic(this, &UBTS_Strafe::OnQueryFinished);
}


void UBTS_Strafe::OnQueryFinished(
    UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    TArray<FVector> ResultLocations;

    if (QueryInstance->GetQueryResultsAsLocations(ResultLocations))
    {
        OwnerController->MoveToLocation(ResultLocations[0], 20.0f, false);
    }
    else
    {
        OwnerController->StopMovement();
    }
}


UEnvQuery* UBTS_Strafe::GetStrafeQuery(const UBehaviorTreeComponent& OwnerBTree) const
{
    AActor* Target = Cast<AActor>(OwnerBTree.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

    if (GameUtils::IsValid(Target))
    {
        if (GameUtils::IsValid(ControlledPawn))
        {
            FVector A = Target->GetActorRightVector();
            FVector B = UKismetMathLibrary::GetDirectionUnitVector(
                Target->GetActorLocation(), ControlledPawn->GetActorLocation());

            A.Z = 0.0f;
            B.Z = 0.0f;
            A.Normalize();
            B.Normalize();
            float Dot = FVector::DotProduct(A, B);
            if (Dot >= -0.1)
            {
                return EQS_StrafeLeftObject;
            }
            else
            {
                return EQS_StrafeRightObject;
            }
        }
    }

    return nullptr;
}