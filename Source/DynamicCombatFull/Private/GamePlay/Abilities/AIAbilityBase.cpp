// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAbilityBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GamePlay/AI/AICharacter.h"
#include "GamePlay/AI/BaseAIController.h"

void AAIAbilityBase::NativeInit(UAbilityComponent* InAbilityComponent)
{
    Super::NativeInit(InAbilityComponent);
    AICharacter = Cast<AAICharacter>(Character);
}

FVector AAIAbilityBase::GetTargetLoc() const
{
    AActor* Target = GetTarget();

    if (Target->IsValidLowLevel())
    {
        return Target->GetActorLocation();
    }

    return GetActorLocation();
}

AActor* AAIAbilityBase::GetTarget() const
{
    if (AICharacter->IsValidLowLevel())
    {
        if (AICharacter->GetAIController()->IsValidLowLevel())
        {
            ABaseAIController* AICon = AICharacter->GetAIController();

            if (AICon->GetTarget()->IsValidLowLevel())
            {
                return AICon->GetTarget();
            }
        }
    }
    
    return nullptr;
}

FTransform AAIAbilityBase::GetSurfaceTargetTransform(float Range) const
{
    FVector EffectLocation = GetTargetLoc();
    FVector ActorLoc = GetOwner()->GetActorLocation();
    FRotator EffectRotation = UKismetMathLibrary::FindLookAtRotation(ActorLoc, EffectLocation);

    float Val = (ActorLoc - EffectLocation).Size();
    float ClampedVal = FMath::Clamp(Val, 0.0f, Range);

    FVector StartLocation = ActorLoc + UKismetMathLibrary::Conv_RotatorToVector(EffectRotation)* ClampedVal;
    FVector OutGroundLocation;
    FVector OutNormal;

    FindGroundLocation(StartLocation, 0.0f, OutGroundLocation, OutNormal);
    EffectLocation = OutGroundLocation;

    return FTransform(EffectRotation, EffectLocation);
}


FTransform AAIAbilityBase::GetSurfacePredictedTargetTransform(float Range) const
{
    AActor* Target = GetTarget();

    if (Target->IsValidLowLevel())
    {
        return GetSurfaceTargetTransform(Range);
    }
    else
    {
        return GetOwner()->GetActorTransform();
    }
}
