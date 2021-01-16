// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireballAbility.h"
#include "Components/AbilityComponent.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/AbilityEffects/FireballProjectileAbilityEffect.h"


APlayerFireballAbility::APlayerFireballAbility()
    :
    InitialProjectileSpeed(3500.0f)
{
    FireballProjectileClass = GameUtils::LoadAssetClass<AFireballProjectileAbilityEffect>(
        "/Game/DynamicCombatSystem/Blueprints/AbilityEffects/FireballProjectileEffectBP");
    
    static UAnimMontage* AnimMontage1 = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_Fireball_01");
    AbilityMontages.Add(AnimMontage1);

    static UAnimMontage* AnimMontage2 = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_Fireball_02");
    AbilityMontages.Add(AnimMontage2);

    static UAnimMontage* AnimMontage3 = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_Fireball_03");
    AbilityMontages.Add(AnimMontage3);


    ManaCost = 15.0f;
    bIsUsingCrosshair = true;
    Damage = 10.0f;
    OwnerDamageScalar = 0.5f;


}

void APlayerFireballAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        UpdateSocket();
        UAnimMontage* AnimMontage = GetAbilityMontage(MontageCounter);
        if (GameUtils::IsValid(AnimMontage))
        {
            float Duration = PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
            UpdateMontageCounter();

            GameUtils::SetTimerRetriggerable(
                GetWorld()->GetTimerManager(),
                RetriggerableTimerHandle,
                FTimerDelegate::CreateUObject(this, &APlayerFireballAbility::Delayed_Released),
                Duration,
                false);
        }
    }

    
}

void APlayerFireballAbility::Effect()
{
    FName SpawnSocket = bUseLeftHandSocket ? GetLeftHandSocket() : GetRightHandSocket();

    EffectTransform = GetCrosshairTransform(SpawnSocket);

    FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
    SpawnParameters.Owner = GetOwner();
    SpawnParameters.Instigator = GetOwner()->GetInstigator();
    SpawnParameters.SpawnCollisionHandlingOverride = 
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AFireballProjectileAbilityEffect* SpawnedActor =
        GetWorld()->SpawnActor<AFireballProjectileAbilityEffect>(
            FireballProjectileClass, EffectTransform, SpawnParameters);

    if (GameUtils::IsValid(SpawnedActor))
    {
        SpawnedActor->Init(GetDamage(), InitialProjectileSpeed, 10.0f, nullptr, true);
        ConsumeMana();
    }
}

void APlayerFireballAbility::ResetMontageCounter()
{
    MontageCounter = 0;
}

void APlayerFireballAbility::UpdateMontageCounter()
{
    MontageCounter++;
    int Num = GetAbilityMontages().Num();
    if (MontageCounter >= Num)
    {
        MontageCounter = 1;
    }
}

void APlayerFireballAbility::UpdateSocket()
{
    if (MontageCounter == 0)
    {
        bUseLeftHandSocket = false;
    }
    else if (MontageCounter == 1)
    {
        bUseLeftHandSocket = true;
    }
    else if (MontageCounter == 2)
    {
        bUseLeftHandSocket = false;
    }
}

void APlayerFireballAbility::Delayed_Released()
{
    ResetMontageCounter();
}