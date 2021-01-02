// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageAbilityEffectBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

#include "Interfaces/CanBeAttacked.h"
#include "Components/EffectsComponent.h"
#include "Components/BehaviorComponent.h"
#include "GameCore/GameUtils.h"

// Sets default values
ADamageAbilityEffectBase::ADamageAbilityEffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Damage = 15.0f;
    Impulse = 20000.0f;
}

void ADamageAbilityEffectBase::Init(float InDamageRadius, float InDamage, float InImpulse)
{
    DamageRadius = InDamageRadius;
    Damage = InDamage;
    Impulse = InImpulse;

    DamageToShapeArea();
    Destroy();
}

void ADamageAbilityEffectBase::DamageToShapeArea()
{
    TArray<FHitResult> OutHits;
    GetTraceObjects(OutHits);

    TArray<AActor*> HitActors;

    for (const FHitResult& Hit : OutHits)
    {
        AActor* HitActor = Hit.GetActor();

        // check whether this actor wars already hit
        if (!HitActors.Contains(HitActor))
        {
            HitActors.Add(HitActor);

            if (IsEnemy(HitActor))
            {
                ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(HitActor);
                if (CanBeAttacked != nullptr)
                {
                    FHitData HitData;
                    HitData.Damage = Damage;
                    HitData.DamageCauser = GetOwner();
                    HitData.HitFromDirection =
                        UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), HitActor->GetActorLocation());

                    HitData.bCanBeParried = false;
                    HitData.bCanBeBlocked = true;
                    HitData.bCanReceiveImpact = false;

                    EAttackResult AttackResult;
                    bool bResult = CanBeAttacked->TakeDamage(HitData, AttackResult);
                    if (bResult)
                    {
                        UEffectsComponent* EffectsComponent =
                            Cast<UEffectsComponent>(HitActor->GetComponentByClass(UEffectsComponent::StaticClass()));

                        if (GameUtils::IsValid(EffectsComponent))
                        {
                            EffectsComponent->ApplyEffect(EEffectType::Stun, 1.0f, EApplyEffectMethod::Replace, GetOwner());
                        }

                        ApplyImpulseToCharacter(HitActor, Hit.Normal);
                    }
                }
            }
        }
    }
}

bool ADamageAbilityEffectBase::IsEnemy(AActor* InTarget) const
{
    ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(InTarget);

    if (CanBeAttacked != nullptr)
    {
        if (CanBeAttacked->IsAlive())
        {
            AActor* LOwner = GetOwner();
            UBehaviorComponent* BehaviorComponent =
                Cast<UBehaviorComponent>(LOwner->GetComponentByClass(UBehaviorComponent::StaticClass()));

            if (GameUtils::IsValid(BehaviorComponent))
            {
                return BehaviorComponent->IsEnemy(InTarget);
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

void ADamageAbilityEffectBase::ApplyImpulseToCharacter(AActor* InActor, FVector Normal)
{
    ACharacter* Character = Cast<ACharacter>(InActor);

    if (GameUtils::IsValid(Character))
    {
        if (Character->GetMesh()->IsAnySimulatingPhysics())
        {
            Character->GetMesh()->AddImpulse(Normal * (-1) * Impulse);
        }
    }
}
