// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAbilityBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GamePlay/AI/AICharacter.h"
#include "GamePlay/AI/BaseAIController.h"
#include "GameCore/GameUtils.h"

void AAIAbilityBase::NativeInit(UAbilityComponent* InAbilityComponent)
{
    Super::NativeInit(InAbilityComponent);
    AICharacter = Cast<AAICharacter>(Character);
}

FVector AAIAbilityBase::GetTargetLoc() const
{
    AActor* Target = GetTarget();

    if (GameUtils::IsValid(Target))
    {
        return Target->GetActorLocation();
    }

    return GetActorLocation();
}

AActor* AAIAbilityBase::GetTarget() const
{
    if (GameUtils::IsValid(AICharacter))
    {
        if (GameUtils::IsValid(AICharacter->GetAIController()))
        {
            ABaseAIController* AICon = AICharacter->GetAIController();

            if (GameUtils::IsValid(AICon->GetTarget()))
            {
                return AICon->GetTarget();
            }
        }
    }
    
    return nullptr;
}

FTransform AAIAbilityBase::GetSurfaceTargetTransform(float InRange) const
{
    FVector EffectLocation = GetTargetLoc();
    FVector ActorLoc = GetOwner()->GetActorLocation();
    FRotator EffectRotation = UKismetMathLibrary::FindLookAtRotation(ActorLoc, EffectLocation);

    float Val = (ActorLoc - EffectLocation).Size();
    float ClampedVal = FMath::Clamp(Val, 0.0f, InRange);

    FVector StartLocation = ActorLoc + UKismetMathLibrary::Conv_RotatorToVector(EffectRotation)* ClampedVal;
    FVector OutGroundLocation;
    FVector OutNormal;

    FindGroundLocation(StartLocation, 0.0f, OutGroundLocation, OutNormal);
    EffectLocation = OutGroundLocation;

    return FTransform(EffectRotation, EffectLocation);
}


FTransform AAIAbilityBase::GetSurfacePredictedTargetTransform(float InRange) const
{
    AActor* Target = GetTarget();

    if (GameUtils::IsValid(Target))
    {
        return GetSurfaceTargetTransform(InRange);
    }
    else
    {
        return GetOwner()->GetActorTransform();
    }
}
