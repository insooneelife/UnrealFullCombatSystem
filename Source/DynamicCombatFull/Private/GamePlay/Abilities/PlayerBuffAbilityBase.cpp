// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBuffAbilityBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AbilityComponent.h"
#include "GamePlay/BaseCharacter.h"
#include "GamePlay/AbilityEffects/BuffAbilityEffect.h"
#include "GameCore/GameUtils.h"

APlayerBuffAbilityBase::APlayerBuffAbilityBase()
{
    BuffValue = 10.0f;
    BuffDuration = 30.0f;
    BuffColor = FLinearColor(50.0f, 1.0f, 1.0f, 1.0f);

    BuffCastParticle = GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_ApplyBuff"));
    Sound = GameUtils::LoadAssetObject<USoundBase>(TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_Buff"));
}

void APlayerBuffAbilityBase::Released()
{
    if (AbilityComponent->StartAbility())
    {
        PlayAbilityMontage(GetAbilityMontage(0), TEXT("None"), true, 1.0f);
    }

    SpawnBuffAbilityEffectClass = GameUtils::LoadAssetClass<ABuffAbilityEffect>(
            TEXT("/Game/DynamicCombatSystem/Blueprints/AbilityEffects/AE_Buff"));
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
    UGameplayStatics::SpawnEmitterAttached(BuffCastParticle, GetPlayerCharacter()->GetMesh());
}

void APlayerBuffAbilityBase::PlaySound()
{
    UGameplayStatics::SpawnSoundAttached(Sound, GetPlayerCharacter()->GetMesh());
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
            ABuffAbilityEffect* Buff = Cast<ABuffAbilityEffect>(Actor);

            if (Buff == nullptr)
                continue;

            if (Actor->GetOwner() == GetOwner())
            {
                if (Buff->GetStatType() == StatType)
                {
                    Buff->AdjustBuff(BuffValue, BuffDuration);
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

        BuffAbility->SetDuration(BuffDuration);
        BuffAbility->SetStatType(StatType);
        BuffAbility->SetValue(BuffValue);
        BuffAbility->SetColor(BuffColor);
    }
}