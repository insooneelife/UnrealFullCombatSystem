// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBuffAbilityBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"
#include "Components/AbilityComponent.h"
#include "GamePlay/BaseCharacter.h"
#include "GamePlay/AbilityEffects/BuffAbilityEffect.h"
#include "GameCore/GameUtils.h"

APlayerBuffAbilityBase::APlayerBuffAbilityBase()
{
    static UAnimMontage* LoadedAnimMontageObject =
        GameUtils::LoadAssetObject<UAnimMontage>(TEXT("/Game/DynamicCombatSystem/Montages/Player/Magic/M_MG_Buff"));

    AbilityMontages = { LoadedAnimMontageObject };

    bRotateOnPressed = false;
    ManaCost = 40.0f;

    BuffValue = 10.0f;
    BuffDuration = 30.0f;
    BuffColor = FLinearColor(50.0f, 1.0f, 1.0f, 1.0f);

    static UParticleSystem* LoadedParticleObject =
        GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_ApplyBuff"));
    BuffCastParticle = LoadedParticleObject;

    static USoundBase* LoadedSoundObject =
        GameUtils::LoadAssetObject<USoundBase>(TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_Buff"));
    Sound = LoadedSoundObject;
}

void APlayerBuffAbilityBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    BuffCastParticle = nullptr;
    Sound = nullptr;
}

void APlayerBuffAbilityBase::Released()
{
    if (AbilityComponent.IsValid())
    {
        if (AbilityComponent->StartAbility())
        {
            PlayAbilityMontage(GetAbilityMontage(0), TEXT("None"), true, 1.0f);
        }
    }
}

void APlayerBuffAbilityBase::Effect()
{
    ApplyBuff();
    SpawnParticle();
    PlaySound();
    ConsumeMana();
}

void APlayerBuffAbilityBase::SpawnParticle()
{
    if (PlayerCharacter.IsValid())
    {
        UGameplayStatics::SpawnEmitterAttached(BuffCastParticle, PlayerCharacter->GetMesh());
    }
}

void APlayerBuffAbilityBase::PlaySound()
{
    if (PlayerCharacter.IsValid())
    {
        UGameplayStatics::SpawnSoundAttached(Sound, PlayerCharacter->GetMesh());
    }
}

void APlayerBuffAbilityBase::ApplyBuff()
{
    if (StatType != EStat::None && 
        BuffValue > 0.0f && 
        BuffDuration > 0.0f)
    {
        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuffAbilityEffect::StaticClass(), OutActors);

        for (AActor* Actor : OutActors)
        {
            ABuffAbilityEffect* AbilityActor = Cast<ABuffAbilityEffect>(Actor);

            if (AbilityActor == nullptr)
            {
                continue;
            }

            if (AbilityActor->GetOwner() == GetOwner())
            {
                if (AbilityActor->GetStatType() == StatType)
                {
                    AbilityActor->AdjustBuff(BuffValue, BuffDuration);
                    return;
                }
            }
        }

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Params.Owner = GetOwner();
        Params.Instigator = GetOwner()->GetInstigator();

        ABuffAbilityEffect* BuffAbility = GetWorld()->SpawnActor<ABuffAbilityEffect>(
            SpawnBuffAbilityEffectClass,
            GetOwner()->GetActorTransform(), 
            Params);

        BuffAbility->Init(BuffDuration, StatType, BuffValue, BuffColor);
    }
}