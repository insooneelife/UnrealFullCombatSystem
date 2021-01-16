// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfernoAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimMontage.h"
#include "GameCore/GameUtils.h"
#include "Components/AbilityComponent.h"
#include "GamePlay/AbilityEffects/DamageSphereAbilityEffect.h"

APlayerInfernoAbility::APlayerInfernoAbility()
    :
    Range(3000.0f),
    Radius(250.0f),
    MaxSurfaceAngle(35.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_InfernoAttack");
    AbilityMontages.Add(AnimMontage);

    ManaCost = 25.0f;
    Damage = 25.0f;
    ImpulsePower = 40000.0f;

    static UParticleSystem* LoadedParticle =
        GameUtils::LoadAssetObject<UParticleSystem>("/Game/DynamicCombatSystem/VFX/P_Explosion");
    HitParticle = LoadedParticle;

    static USoundBase* LoadedSound =
        GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/CUE_GroundExplosion");
    Sound = LoadedSound;

    DamageSphereEffectClass = GameUtils::LoadAssetClass<ADamageSphereAbilityEffect>(
        "/Game/DynamicCombatSystem/Blueprints/AbilityEffects/DamageSphereEffectBP");
}

void APlayerInfernoAbility::Tick(float DeltaTime)
{
    if (AbilityComponent->IsPressed() &&
        !AbilityComponent->IsCasting())
    {
        UpdateEffect();
    }
}

void APlayerInfernoAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);

        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
            UpdateEffect();
        }
    }
    else
    {
        if (!AbilityComponent->IsCasting())
        {
            HideIndicator();
        }
    }
}

void APlayerInfernoAbility::Effect()
{
    FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
    SpawnParameters.Owner = GetOwner();
    SpawnParameters.Instigator = GetOwner()->GetInstigator();
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ADamageSphereAbilityEffect* SpawnedActor =
        GetWorld()->SpawnActor<ADamageSphereAbilityEffect>(
            DamageSphereEffectClass, EffectTransform, SpawnParameters);

    if (GameUtils::IsValid(SpawnedActor))
    {
        SpawnedActor->Init(Radius, GetDamage(), GetImpulsePower(), true);
        SpawnParticle();
        ConsumeMana();
        PlaySound();
    }
}

void APlayerInfernoAbility::Pressed()
{
    if (!AbilityComponent->IsCasting())
    {
        UpdateEffect();
        ShowIndicator(EffectTransform.GetLocation());
    }
}

void APlayerInfernoAbility::UpdateEffect()
{
    EffectTransform = GetSurfaceTransform(Range, MaxSurfaceAngle, 2000.0f);
    UpdateIndicator(EffectTransform.GetLocation());
}

void APlayerInfernoAbility::SpawnParticle()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetEffectLocation());
}

void APlayerInfernoAbility::PlaySound()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, EffectTransform.GetLocation());
}