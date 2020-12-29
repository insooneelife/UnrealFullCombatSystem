// Fill out your copyright notice in the Description page of Project Settings.


#include "FireballProjectileAbilityEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/BehaviorComponent.h"
#include "Components/EffectsComponent.h"
#include "Interfaces/CanBeAttacked.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"

// Sets default values
AFireballProjectileAbilityEffect::AFireballProjectileAbilityEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    InitialSpeed = 3500.0f;
    LifeTime = 10.0f;
    bApplyStun = true;
    HeadShotDamageMultiplier = 2.0f;
    HomingInterpSpeed = 40.0f;
    StopHomingDistance = 300.0f;
    ImpulsePower = 30000.0f;

    CollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>("CollisionHandler");
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
    RootComponent = CollisionSphere = CreateDefaultSubobject<UStaticMeshComponent>("CollisionSphere");
    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");

    static UParticleSystem* LoadedParticleObject = 
        GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_FireballHit"));
    HitParticle = LoadedParticleObject;

    static USoundBase* LoadedSoundObject =
        GameUtils::LoadAssetObject<USoundBase>(TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_FireballHit"));
    HitSound = LoadedSoundObject;

    static UParticleSystem* LoadedFireballParticleObject =
        GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_FireballHit"));
    FireballHitParticle = LoadedFireballParticleObject;
}

// Called when the game starts or when spawned
void AFireballProjectileAbilityEffect::BeginPlay()
{
	Super::BeginPlay();
    CollisionHandler->OnHit.AddDynamic(this, &AFireballProjectileAbilityEffect::OnHit);
    ProjectileMovement->Velocity = GetActorForwardVector() * InitialSpeed;

    EnableHomingProjectile();
    SetLifeSpan(LifeTime);

    CollisionHandler->SetCollisionMesh(CollisionSphere, CollisionSphere->GetAllSocketNames());
    CollisionHandler->ActivateCollision(ECollisionPart::None);
}

void AFireballProjectileAbilityEffect::EnableHomingProjectile()
{
    if (HomingTarget->IsValidLowLevel())
    {
        GetWorld()->GetTimerManager().SetTimer(
            UpdateHomingProjectileTimerHandle,
            this,
            &AFireballProjectileAbilityEffect::UpdateHomingProjectile,
            0.016f,
            true);
    }
}

void AFireballProjectileAbilityEffect::DisableHomingProjectile()
{
    GetWorld()->GetTimerManager().ClearTimer(UpdateHomingProjectileTimerHandle);
}

void AFireballProjectileAbilityEffect::ApplyHitImpulse(UPrimitiveComponent* Component, FVector HitNormal, FName BoneName)
{
    if (Component->IsAnySimulatingPhysics())
    {
        Component->AddImpulse(HitNormal * (-1.0f) * ImpulsePower, BoneName);
    }
}

void AFireballProjectileAbilityEffect::OnHit(const FHitResult& Hit)
{
    FVector HitLoc = Hit.Location;
    FVector HitNormal = Hit.Normal;
    AActor* HitActor = Hit.GetActor();
    UPrimitiveComponent* HitComp = Hit.GetComponent();
    FName HitBoneName = Hit.BoneName;


    if (GetOwner() != HitActor)
    {
        ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(HitActor);

        if (CanBeAttacked != nullptr)
        {
            if (IsEnemy(HitActor))
            {
                FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(HitLoc, HitActor->GetActorLocation());
                FHitData HitData(Damage, GetOwner(), Direction, false, true, false);

                EAttackResult OutResult;

                bool bResult = CanBeAttacked->TakeDamage(HitData, OutResult);

                if (OutResult != EAttackResult::Failed)
                {
                    if (bResult)
                    {
                        UDefaultGameInstance* GameInst = Cast<UDefaultGameInstance>(GetGameInstance());

                        if (GameInst->IsValidLowLevel())
                        {
                            GameInst->PlayHitSound(GetOwner(), HitActor, HitLoc);
                            if (bApplyStun)
                            {
                                UEffectsComponent* EffectsComponent =
                                    Cast<UEffectsComponent>(
                                        HitActor->GetComponentByClass(UEffectsComponent::StaticClass()));

                                if (EffectsComponent->IsValidLowLevel())
                                {
                                    EffectsComponent->ApplyEffect(
                                        EEffectType::Stun, 1.0f, EApplyEffectMethod::Replace, GetOwner());
                                }

                            }
                        }
                    }

                    
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitLoc);
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, FTransform(FQuat::Identity, HitLoc));
                    ApplyHitImpulse(HitComp, HitNormal, HitBoneName);
                    Destroy();
                }
            }
        }

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitLoc);
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireballHitParticle, FTransform(FQuat::Identity, HitLoc));
        ApplyHitImpulse(HitComp, HitNormal, HitBoneName);
        Destroy();
    }
}

bool AFireballProjectileAbilityEffect::IsEnemy(AActor* Target) const
{
    UBehaviorComponent* BehaviorComp = 
        Cast<UBehaviorComponent>(GetOwner()->GetComponentByClass(UBehaviorComponent::StaticClass()));
    if (BehaviorComp->IsValidLowLevel())
    {
        return BehaviorComp->IsEnemy(Target);
    }
    else
    {
        return true;
    }
}

void AFireballProjectileAbilityEffect::UpdateHomingProjectile()
{
    if (HomingTarget->IsValidLowLevel())
    {
        float Distance = (HomingTarget->GetActorLocation() - GetActorLocation()).Size();

        if (Distance <= StopHomingDistance)
        {
            DisableHomingProjectile();
        }
        else
        {
            FVector HomingLoc = HomingTarget->GetActorLocation();
            FVector Vel = HomingTarget->GetVelocity()* (Distance / InitialSpeed);

            FRotator Current = GetActorRotation();
            FRotator Target = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HomingLoc + Vel);

            FRotator Rot = 
                UKismetMathLibrary::RInterpTo_Constant(Current, Target, GetWorld()->GetDeltaSeconds(), HomingInterpSpeed);

            ProjectileMovement->Velocity = UKismetMathLibrary::Conv_RotatorToVector(Rot) * InitialSpeed;

        }
    }
    else
    {
        DisableHomingProjectile();
    }
}


