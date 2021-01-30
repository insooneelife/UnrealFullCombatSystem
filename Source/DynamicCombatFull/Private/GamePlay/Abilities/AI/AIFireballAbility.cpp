// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFireballAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/AbilityComponent.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/AbilityEffects/FireballProjectileAbilityEffect.h"

AAIFireballAbility::AAIFireballAbility()
{
    ManaCost = 15.0f;
    Damage = 20.0f;

    static UAnimMontage* FireballAnimMontage1 = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_Fireball_01");

    static UAnimMontage* FireballAnimMontage2 = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_Fireball_02");

    static UAnimMontage* FireballAnimMontage3 = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_Fireball_03");

    AbilityMontages.Add(FireballAnimMontage1);
    AbilityMontages.Add(FireballAnimMontage2);
    AbilityMontages.Add(FireballAnimMontage3);
}

void AAIFireballAbility::Released()
{
    Super::Released();

    bool bStartAbility = AbilityComponent->StartAbility();

    if (bStartAbility)
    {
        StartTimer();
        UpdateSocket();

        UAnimMontage* AnimMontage = GetAbilityMontage(MontageCounter);

        if (GameUtils::IsValid(AnimMontage))
        {
            float Duration = PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
            UpdateMontageCounter();

            GameUtils::SetTimerRetriggerable(
                GetWorld()->GetTimerManager(),
                RetriggerableTimerHandle,
                FTimerDelegate::CreateUObject(this, &AAIFireballAbility::Delayed_ResetMontageCounter),
                Duration,
                false);
        }
    }
}

void AAIFireballAbility::Effect()
{
    Super::Effect();

    UpdateEffect();

    FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
    SpawnParameters.Owner = GetOwner()->GetInstigator();
    SpawnParameters.Instigator = GetOwner()->GetInstigator();
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AFireballProjectileAbilityEffect* SpawnedActor =
        GetWorld()->SpawnActor<AFireballProjectileAbilityEffect>(
            FireballProjectileClass, EffectTransform, SpawnParameters);

    const float InitialSpeed = 1500.0f;
    const float LifeTime = 10.0f;
    SpawnedActor->Init(GetDamage(), InitialSpeed, LifeTime, GetTarget(), false);
    StopTimer();
    ConsumeMana();
}

void AAIFireballAbility::Ended(EAbilityEndResult Result)
{
    Super::Ended(Result);
    StopTimer();
}

void AAIFireballAbility::UpdateEffect()
{
    FName HandSocketName = bUseLeftHandSocket ? GetLeftHandSocket(): GetRightHandSocket();

    FVector Start = Character->GetMesh()->GetSocketLocation(HandSocketName);
    FVector Target = GetTargetLoc();
    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
    FVector DirVec = UKismetMathLibrary::Conv_RotatorToVector(LookAtRot);
    EffectTransform = FTransform(LookAtRot, Start + DirVec);

}

void AAIFireballAbility::ResetMontageCounter()
{
    MontageCounter = 0;
}

void AAIFireballAbility::UpdateMontageCounter()
{
    MontageCounter++;

    int Num = GetAbilityMontages().Num();

    if (MontageCounter >= Num)
    {
        MontageCounter = 1;
    }
}

void AAIFireballAbility::UpdateSocket()
{
    if (MontageCounter == 0)
    {
        bUseLeftHandSocket = false;
    }
    else if (MontageCounter == 1)
    {
        bUseLeftHandSocket = true;
    }
    else
    {
        bUseLeftHandSocket = false;
    }
}

void AAIFireballAbility::StartTimer()
{
    GetWorld()->GetTimerManager().SetTimer(UpdateEffectTimerHandle, this, &AAIFireballAbility::UpdateEffect, 0.016f, true);
}

void AAIFireballAbility::StopTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(UpdateEffectTimerHandle);
}

void AAIFireballAbility::Delayed_ResetMontageCounter()
{
    ResetMontageCounter();
}