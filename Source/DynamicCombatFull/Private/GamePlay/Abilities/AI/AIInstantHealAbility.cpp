// Fill out your copyright notice in the Description page of Project Settings.


#include "AIInstantHealAbility.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "GamePlay/AI/AICharacter.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/AbilityComponent.h"

AAIInstantHealAbility::AAIInstantHealAbility()
    :
    HealAmount(45.0f)
{
    ManaCost = 20.0f;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_InstantHeal");
    AbilityMontages.Add(AnimMontage);

    static UParticleSystem* LoadedParticle = 
        GameUtils::LoadAssetObject<UParticleSystem>("/Game/DynamicCombatSystem/VFX/P_InstantHeal");
    HealParticle = LoadedParticle;

    static USoundBase* LoadedSound = 
        GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/CUE_PotionHeal");
    Sound = LoadedSound;
}

void AAIInstantHealAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    HealParticle = nullptr;
    Sound = nullptr;
}

void AAIInstantHealAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);
        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
        }
    }
}

void AAIInstantHealAbility::Effect()
{
    Heal();
    PlaySound();
    SpawnParticle();
    ConsumeMana();
}

void AAIInstantHealAbility::SpawnParticle()
{
    UGameplayStatics::SpawnEmitterAttached(HealParticle, GetCharacter()->GetMesh());
}

void AAIInstantHealAbility::PlaySound()
{
    UGameplayStatics::SpawnSoundAttached(Sound, GetCharacter()->GetMesh());
}


void AAIInstantHealAbility::Heal()
{
    UExtendedStatComponent* HealthStatComp =
        UDefaultGameInstance::GetExtendedStatComponent(GetAICharacter(), EStat::Health);

    if (GameUtils::IsValid(HealthStatComp))
    {
        HealthStatComp->ModifyStat(HealAmount, false);
    }
}