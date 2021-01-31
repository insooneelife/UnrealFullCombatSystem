// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerVortexAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"
#include "Components/AbilityComponent.h"
#include "Interfaces/CanBeAttacked.h"

APlayerVortexAbility::APlayerVortexAbility()
    :
    Range(1500.0f),
    Radius(100.0f),
    MaxSurfaceAngle(30.0f),
    VortexSpeed(250.0f),
    CollisionHeight(250.0f),
    FirstUseManaCost(15.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_BeamAttack");
    AbilityMontages.Add(AnimMontage);

    bCanBeCancelled = true;
    bRotateOnPressed = true;
    ManaCost = 0.1f;
    Damage = 0.25f;
    bIncludeOwnerDamage = true;
    OwnerDamageScalar = 0.01f;
    ImpulsePower = 50000.0f;

    static UParticleSystem* LoadedParticle =
        GameUtils::LoadAssetObject<UParticleSystem>("/Game/DynamicCombatSystem/VFX/P_Vortex");
    VortexParticle = LoadedParticle;

    static USoundBase* LoadedSound =
        GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/CUE_Tornado");
    Sound = LoadedSound;

    IndicatorRadius = 100.0f;
}

void APlayerVortexAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    VortexParticle = nullptr;
    Sound = nullptr;
    ParticleComponent = nullptr;
    SoundComponent = nullptr;
}

void APlayerVortexAbility::Tick(float DeltaTime)
{
    if (AbilityComponent->IsUsingAbility())
    {
        EffectTransform = GetSurfaceTransform(Range, MaxSurfaceAngle, 2000.0f);

        AbilityComponent->UpdateSpellIndicatorLocation(EffectTransform.GetLocation());

        if (bEffectEnabled)
        {
            UpdateVortex();
            AttackToEnemies();
            ConsumeMana();
        }
    }
}

void APlayerVortexAbility::Released()
{
    ClearAttemptToCastTimer();
    if (AbilityComponent->IsCasting())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);

        PlayAbilityMontage(AnimMontage, FName("End"), true, 1.0f);
        AbilityComponent->EndAbility(EAbilityEndResult::Finished);
    }
}

void APlayerVortexAbility::Pressed()
{
    ShowIndicator(EffectTransform.GetLocation());
    if (!AttemptToCast())
    {
        GetWorld()->GetTimerManager().SetTimer(
            AttempToCastTimerHandle, this, &APlayerVortexAbility::AttempToCastEvent, 0.1f, true);
    }
}

void APlayerVortexAbility::Effect()
{
    bEffectEnabled = true;
    VortexLocation = EffectTransform.GetLocation();
    PlaySound();
    SpawnParticle();
    ConsumeManaAmount(FirstUseManaCost);
}

void APlayerVortexAbility::Ended(EAbilityEndResult Result)
{
    ClearAttemptToCastTimer();
    bEffectEnabled = false;
    DeactivateParticle();
    StopSound();

    if (EAbilityEndResult::OutOfMana == Result)
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);
        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, FName("End"), true, 1.0f);
        }
    }
}

void APlayerVortexAbility::AttempToCastEvent()
{
    AttemptToCast();
}

void APlayerVortexAbility::ClearAttemptToCastTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(AttempToCastTimerHandle);
}

void APlayerVortexAbility::SpawnParticle()
{
    if (ParticleComponent != nullptr)
    {
        ParticleComponent->Activate(true);
        ParticleComponent->SetWorldLocation(VortexLocation);
    }
    else
    {
        ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VortexParticle, VortexLocation);
    }
}

void APlayerVortexAbility::DeactivateParticle()
{
    if (ParticleComponent != nullptr)
    {
        ParticleComponent->Deactivate();
    }
}

void APlayerVortexAbility::PlaySound()
{
    if (SoundComponent != nullptr)
    {
        SoundComponent->SetWorldLocation(VortexLocation);
    }
    else
    {
        SoundComponent = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, VortexLocation);
    }

    SoundComponent->FadeIn(0.5f, 1.0f);
}

void APlayerVortexAbility::StopSound()
{
    if (SoundComponent != nullptr)
    {
        SoundComponent->FadeOut(0.5f, 0.0f);
    }
}

void APlayerVortexAbility::UpdateVortex()
{
    FVector Target = EffectTransform.GetLocation();
    float Delta = GetWorld()->GetDeltaSeconds();

    VortexLocation = UKismetMathLibrary::VInterpTo_Constant(VortexLocation, Target, Delta, VortexSpeed);

    GameUtils::DrawPoint(GetWorld(), EffectTransform.GetLocation(), FColor::Blue);

    if (ParticleComponent != nullptr)
    {
        ParticleComponent->SetWorldLocationAndRotation(VortexLocation, FQuat::Identity);
    }

    if (SoundComponent != nullptr)
    {
        SoundComponent->SetWorldLocation(VortexLocation);
    }
}

void APlayerVortexAbility::AttackToEnemies()
{
    FVector WorldLocation = ParticleComponent->GetComponentLocation();
    WorldLocation.Z += CollisionHeight;

    FVector Start = WorldLocation;
    FVector End = WorldLocation + FVector(0, 0, 1);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes =
    {
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
    };

    TArray<AActor*> ActorsToIgnore{ GetOwner() };
    TArray<FHitResult> HitResults;

    UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(),
        Start, 
        End, 
        Radius, 
        CollisionHeight,
        ObjectTypes, 
        false, 
        ActorsToIgnore,
        EDrawDebugTrace::Type::None,
        HitResults,
        true);

    TArray<AActor*> HitActors;

    for (const FHitResult& HitResult : HitResults)
    {
        AActor* HitActor = HitResult.GetActor();
        FVector HitNormal = HitResult.Normal;


        if (!HitActors.Contains(HitActor))
        {
            HitActors.Add(HitActor);

            ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(HitActor);
            if (CanBeAttacked != nullptr)
            {
                FHitData HitData;
                HitData.Damage = GetDamage();
                HitData.DamageCauser = GetOwner();
                HitData.HitFromDirection =
                    UKismetMathLibrary::GetDirectionUnitVector(
                        GetOwner()->GetActorLocation(), HitActor->GetActorLocation());

                HitData.bCanBeParried = false;
                HitData.bCanBeBlocked = true;
                HitData.bCanReceiveImpact = false;

                EAttackResult Result;
                CanBeAttacked->TakeDamage(HitData, Result);
                ApplyImpulseToCharacter(HitActor, HitNormal, GetImpulsePower());
            }
        }

    }
}

bool APlayerVortexAbility::AttemptToCast()
{
    if (AbilityComponent->StartAbility())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);
        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
            ClearAttemptToCastTimer();
            return true;
        }
    }
    return false;
}
