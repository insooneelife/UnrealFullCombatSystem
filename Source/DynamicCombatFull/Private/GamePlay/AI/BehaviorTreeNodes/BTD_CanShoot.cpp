// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CanShoot.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlueprintNodeHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interfaces/IsArcher.h"
#include "GameCore/GameUtils.h"

bool UBTD_CanShoot::PerformConditionCheckAI(
    UBehaviorTreeComponent& InOwnerComp,
    AAIController* InOwnerController, 
    APawn* InControlledPawn) const
{
    const float MinDotProduct = 0.75f;
    APawn* ControlledPawn = InControlledPawn;
    AActor* Target = Cast<AActor>(InOwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

    if (GameUtils::IsValid(Target))
    {
        FVector Start = ControlledPawn->GetActorLocation();
        FVector End = Target->GetActorLocation();

        TArray<AActor*> IgnoredActors = { ControlledPawn, Target };
        TArray<FHitResult> HitResults;
        TEnumAsByte<ETraceTypeQuery> TraceType = 
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
        
        if (UKismetSystemLibrary::SphereTraceMulti(
            GetWorld(),
            Start, End, 30.0f, TraceType, false, IgnoredActors, EDrawDebugTrace::Type::None, HitResults, true))
        {
            return false;
        }
        else
        {
            IIsArcher* IsArcher = Cast<IIsArcher>(ControlledPawn);
            if (IsArcher != nullptr)
            {
                float AimAlpha = IsArcher->GetAimAlpha();
                bool A = UKismetMathLibrary::NearlyEqual_FloatFloat(AimAlpha, 1.0f, 0.01f);
                bool B = ControlledPawn->GetHorizontalDotProductTo(Target) >= MinDotProduct;
                return A && B;
            }
        }
    }
    return false;
}