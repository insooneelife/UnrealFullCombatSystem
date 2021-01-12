// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveArrowProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "GameCore/GameUtils.h"
#include "Interfaces/CanBeAttacked.h"
#include "Components/EffectsComponent.h"


AExplosiveArrowProjectile::AExplosiveArrowProjectile()
    :
    ExplosionRadius(250.0f)
{
    InitialSpeed = 0.0f;
    ImpulsePower = 40000.0f;

    UParticleSystem* LoadedParticleObject = GameUtils::LoadAssetObject<UParticleSystem>(
        "/Game/DynamicCombatSystem/VFX/P_Explosion");
    ExplosionParticleObject = LoadedParticleObject;

    USoundBase* LoadedSoundObject = GameUtils::LoadAssetObject<USoundBase>(
        "/Game/DynamicCombatSystem/SFX/CUE/CUE_Explosion");
    ExplosionSoundObject = LoadedSoundObject;
}


void AExplosiveArrowProjectile::OnArrowHit(const FHitResult& InHit)
{
    FVector HitLocation = InHit.Location;
    AActor* HitActor = InHit.GetActor();



    if (GetOwner() != HitActor)
    {
        FVector Start = HitLocation;
        FVector End = HitLocation + FVector(0.0f, 0.0f, 1.0f);

        TArray<AActor*> IgnoredActors{ GetOwner() };
        TArray<FHitResult> HitResults;

        if (UKismetSystemLibrary::SphereTraceMulti(
            GetWorld(),
            Start, End,
            ExplosionRadius,
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
            false,
            IgnoredActors,
            EDrawDebugTrace::Type::None,
            HitResults,
            true))
        {
            TArray<AActor*> HitActors;

            for (const FHitResult& Hit : HitResults)
            {
                FVector Loc = Hit.Location;
                FVector Normal = Hit.Normal;
                AActor* Actor = Hit.GetActor();
                UPrimitiveComponent* Component = Hit.GetComponent();
                FName BoneName = Hit.BoneName;

                if (!HitActors.Contains(Actor))
                {
                    HitActors.Add(Actor);

                    ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(Actor);
                    if (CanBeAttacked != nullptr)
                    {
                        if (IsEnemy(Actor))
                        {
                            FHitData HitData;
                            HitData.Damage = GetDamage();
                            HitData.DamageCauser = GetOwner();
                            HitData.HitFromDirection =
                                UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Actor->GetActorLocation());

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

                                ApplyHitImpulse(Component, Normal, BoneName);
                            }
                        }
                    }
                }

            }
        }

        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticleObject,
            FTransform(FRotator::ZeroRotator, HitLocation));

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSoundObject, HitLocation);

        Destroy();
    }
}
