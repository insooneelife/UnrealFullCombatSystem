// Fill out your copyright notice in the Description page of Project Settings.


#include "AITrapAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstance.h"
#include "GamePlay/AbilityEffects/DamageTrapAbilityEffect.h"
#include "GameCore/GameUtils.h"
#include "Components/AbilityComponent.h"

AAITrapAbility::AAITrapAbility()
    :
    TrapRadius(150.0f),
    DamageRadius(200.0f),
    Duration(15.0f),
    ActivationDelay(1.0f),
    Range(300.0f)
{
    ManaCost = 15.0f;
    Damage = 25.0f;
    IndicatorRadius = 150.0f;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_SettingTrap");
    AbilityMontages.Add(AnimMontage);


    static UMaterialInstance* LoadedMaterial = GameUtils::LoadAssetObject<UMaterialInstance>(
        "/Game/DynamicCombatSystem/VFX/Materials/MI_SpellIndicatror_03");
    IndicatorMaterial = LoadedMaterial;

    static USoundBase* SoundObject = GameUtils::LoadAssetObject<USoundBase>(
        "/Game/DynamicCombatSystem/SFX/CUE/CUE_Teleport");
    Sound = SoundObject;
}

void AAITrapAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);

        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
            EffectTransform = GetSurfaceTargetTransform(Range);
            ShowIndicator(GetEffectLocation());
        }
    }
}

void AAITrapAbility::Effect()
{
    FActorSpawnParameters Params;
    Params.Owner = GetOwner();
    Params.Instigator = GetOwner()->GetInstigator();
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform SpawnTransform = GetEffectTransform();

    ADamageTrapAbilityEffect* DamageTrapAbilityEffect =
        GetWorld()->SpawnActor<ADamageTrapAbilityEffect>(
            ADamageTrapAbilityEffect::StaticClass(), SpawnTransform, Params);

    if (GameUtils::IsValid(DamageTrapAbilityEffect))
    {
        DamageTrapAbilityEffect->Init(
            DamageRadius, GetDamage(), GetImpulsePower(), TrapRadius, Duration, ActivationDelay);
        PlaySound();
        ConsumeMana();
    }
}

void AAITrapAbility::PlaySound()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetEffectLocation());
}
