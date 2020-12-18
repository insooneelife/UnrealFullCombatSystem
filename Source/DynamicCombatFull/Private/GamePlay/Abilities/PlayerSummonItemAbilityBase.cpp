// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSummonItemAbilityBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AbilityComponent.h"
#include "GamePlay/BaseCharacter.h"

#include "GamePlay/AbilityEffects/SummonedItemAbilityEffect.h"
#include "GameCore/GameUtils.h"

APlayerSummonItemAbilityBase::APlayerSummonItemAbilityBase()
{
    Duration = 60.0f;

    SummonCastParticle = GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_Summon"));
    Sound = GameUtils::LoadAssetObject<USoundBase>(TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_SummonItem"));
}

void APlayerSummonItemAbilityBase::Effect()
{
    Summon();
    SpawnParticle();
    PlaySound();
    ConsumeMana();

    SpawnSummonedItemAbilityEffectClass = GameUtils::LoadAssetClass<ASummonedItemAbilityEffect>(
        TEXT("/Game/DynamicCombatSystem/Blueprints/AbilityEffects/AE_SummonedItem"));
}

void APlayerSummonItemAbilityBase::Released()
{
    if (AbilityComponent->StartAbility())
    {
        PlayAbilityMontage(GetAbilityMontage(0), TEXT("None"), true, 1.0f);
    }
}

void APlayerSummonItemAbilityBase::SpawnParticle()
{
    UGameplayStatics::SpawnEmitterAttached(SummonCastParticle, GetPlayerCharacter()->GetMesh());
}

void APlayerSummonItemAbilityBase::Summon()
{
    for (TSubclassOf<UItemBase> ItemClass : SummonedItemClasses)
    {
        if (UKismetSystemLibrary::IsValidClass(ItemClass))
        {
            SpawnItem(ItemClass);
        }
    }
}

void APlayerSummonItemAbilityBase::PlaySound()
{
    UGameplayStatics::SpawnSoundAttached(Sound, GetPlayerCharacter()->GetMesh());
}

void APlayerSummonItemAbilityBase::SpawnItem(TSubclassOf<UItemBase> Class)
{
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = GetOwner();
    Params.Instigator = GetOwner()->GetInstigator();

    ASummonedItemAbilityEffect* Ability = GetWorld()->SpawnActor<ASummonedItemAbilityEffect>(
        SpawnSummonedItemAbilityEffectClass,
        GetOwner()->GetActorTransform(),
        Params);

    Ability->SetItemClass(Class);
    Ability->SetDuration(Duration);
    Ability->SetAmount(bAutoSwap);
    Ability->SetAutoSwap(bAutoSwap);
}
