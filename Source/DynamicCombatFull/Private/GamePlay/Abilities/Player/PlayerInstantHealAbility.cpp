// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInstantHealAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"
#include "Components/AbilityComponent.h"
#include "Components/ExtendedStatComponent.h"


APlayerInstantHealAbility::APlayerInstantHealAbility()
    :
    HealAmount(30.0f)
{
    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_InstantHeal");
    AbilityMontages.Add(AnimMontage);

    bCanBeCancelled = true;
    bRotateOnPressed = false;
    ManaCost = 30.0f;
    Damage = 0.0f;
    
    static UParticleSystem* LoadedParticle =
        GameUtils::LoadAssetObject<UParticleSystem>("/Game/DynamicCombatSystem/VFX/P_InstantHeal");
    HealCastParticle = LoadedParticle;

    static USoundBase* LoadedSound =
        GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/CUE_PotionHeal");
    Sound = LoadedSound;

}

void APlayerInstantHealAbility::NativeInit(UAbilityComponent* InAbilityComponent)
{
    Super::NativeInit(InAbilityComponent);
    HealthComponent = UDefaultGameInstance::GetExtendedStatComponent(GetOwner(), EStat::Health);
}

void APlayerInstantHealAbility::Released()
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

void APlayerInstantHealAbility::Effect()
{
    Heal();
    SpawnParticle();
    PlaySound();
    ConsumeMana();
}

void APlayerInstantHealAbility::SpawnParticle()
{
    UGameplayStatics::SpawnEmitterAttached(HealCastParticle, GetCharacter()->GetMesh());
}

void APlayerInstantHealAbility::PlaySound()
{
    UGameplayStatics::SpawnSoundAttached(Sound, GetCharacter()->GetMesh());
}

void APlayerInstantHealAbility::Heal()
{
    if (GameUtils::IsValid(HealthComponent))
    {
        HealthComponent->ModifyStat(HealAmount, true);
    }
}
