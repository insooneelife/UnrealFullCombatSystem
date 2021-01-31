// Fill out your copyright notice in the Description page of Project Settings.


#include "AIInfernoAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Materials/MaterialInstance.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"
#include "GamePlay/AbilityEffects/DamageSphereAbilityEffect.h"
#include "Components/AbilityComponent.h"


AAIInfernoAbility::AAIInfernoAbility()
    :
    Radius(300.0f),
    Range(2000.0f)
{
    Damage = 35.0f;
    IndicatorRadius = 300.0f;
    ManaCost = 20.0f;

    static UMaterialInstance* LoadedMaterial =
        GameUtils::LoadAssetObject<UMaterialInstance>("/Game/DynamicCombatSystem/VFX/Materials/MI_SpellIndicatror_03");

    IndicatorMaterial = LoadedMaterial;

    HitParticle = GameUtils::LoadAssetObject<UParticleSystem>("/Game/DynamicCombatSystem/VFX/P_Explosion");
    Sound = GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/CUE_GroundExplosion");

    static UAnimMontage* LoadedMontage = 
        GameUtils::LoadAssetObject<UAnimMontage>("/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_InfernoAttack");

    AbilityMontages.Add(LoadedMontage);
}

void AAIInfernoAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    HitParticle = nullptr;
    Sound = nullptr;
}

void AAIInfernoAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);
        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, FName("None"), true, 1.0f);
            FTransform TargetTransform = GetSurfacePredictedTargetTransform(Range);

            SetEffectTransform(TargetTransform);
            ShowIndicator(GetEffectLocation());
        }
    }
}

void AAIInfernoAbility::Effect()
{
    FActorSpawnParameters Params;
    Params.Owner = GetOwner();
    Params.Instigator = GetOwner()->GetInstigator();
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform SpawnTransform = GetEffectTransform();

    ADamageSphereAbilityEffect* DamageSphereAbilityEffect = 
        GetWorld()->SpawnActor<ADamageSphereAbilityEffect>(
            ADamageSphereAbilityEffect::StaticClass(), SpawnTransform, Params);

    if (GameUtils::IsValid(DamageSphereAbilityEffect))
    {
        DamageSphereAbilityEffect->Init(Radius, GetDamage(), GetImpulsePower(), true);
        SpawnParticle();
        PlaySound();
        ConsumeMana();
    }
}

void AAIInfernoAbility::SpawnParticle()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetEffectLocation());
}

void AAIInfernoAbility::PlaySound()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetEffectLocation());
}
