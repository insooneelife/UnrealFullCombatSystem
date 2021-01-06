// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsSomethingBlocking.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTD_IsSomethingBlocking::UBTD_IsSomethingBlocking()
    :
    Direction(EDirection::Back),
    Distance(200.0f),
    TraceChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility))
{
}

bool UBTD_IsSomethingBlocking::PerformConditionCheckAI(
    UBehaviorTreeComponent& InOwnerComp,
    AAIController* InOwnerController,
    APawn* InControlledPawn) const
{
    FVector Forward = InControlledPawn->GetActorForwardVector();
    FVector Back = Forward * -1.0f;
    FVector Right = InControlledPawn->GetActorRightVector();
    FVector Left = Right * -1.0f;
    FVector DirectionVector;

    if (Direction == EDirection::Front)
    {
        DirectionVector = Forward;
    }
    else if (Direction == EDirection::Back)
    {
        DirectionVector = Back;
    }
    else if (Direction == EDirection::Right)
    {
        DirectionVector = Right;
    }
    else
    {
        DirectionVector = Left;
    }

    FVector Start = InControlledPawn->GetActorLocation();
    FVector End = Start + DirectionVector * Distance;

    TArray<AActor*> IgnoredActors = { InControlledPawn };
    TArray<FHitResult> HitResults;

    return UKismetSystemLibrary::SphereTraceMulti(
        GetWorld(), Start, End, 30.0f, TraceChannel, false, IgnoredActors, DebugType, HitResults, true);

}