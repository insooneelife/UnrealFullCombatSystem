// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTeleportAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"
#include "Components/AbilityComponent.h"

APlayerTeleportAbility::APlayerTeleportAbility()
    :
    Range(2000.0f),
    MaxSurfaceAngle(45.0f)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Magic/M_Magic_Teleport");
    AbilityMontages.Add(AnimMontage);

    bCanBeCancelled = true;
    bRotateOnPressed = true;
    ManaCost = 25.0f;
    Damage = 0.0f;

    static UParticleSystem* LoadedParticle =
        GameUtils::LoadAssetObject<UParticleSystem>("/Game/DynamicCombatSystem/VFX/P_Teleport");
    TeleportParticle = LoadedParticle;

    static USoundBase* LoadedSound =
        GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/CUE_Teleport");
    Sound = LoadedSound;

    IndicatorRadius = 150.0f;
}

void APlayerTeleportAbility::Tick(float DeltaTime)
{
    if (AbilityComponent->IsPressed() && 
        !AbilityComponent->IsCasting())
    {
        UpdateEffect();
    }
}

void APlayerTeleportAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        UAnimMontage* AnimMontage = GetAbilityMontage(0);
        if (GameUtils::IsValid(AnimMontage))
        {
            PlayAbilityMontage(AnimMontage, NAME_None, true, 1.0f);
            UpdateEffect();
        }
        else
        {
            if (!AbilityComponent->IsCasting())
            {
                HideIndicator();
            }
        }
    }
}

void APlayerTeleportAbility::Pressed()
{
    if (!AbilityComponent->IsCasting())
    {
        UpdateEffect();
        ShowIndicator(EffectTransform.GetLocation());
    }
}

void APlayerTeleportAbility::Effect()
{
    if (AbilityTeleport())
    {
        ConsumeMana();
        SpawnTeleportParticle();
        PlaySound();
        AbilityComponent->EndAbility(EAbilityEndResult::Finished);
    }
    else
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Finished);
    }

}

void APlayerTeleportAbility::UpdateEffect()
{
    EffectTransform = GetSurfaceTransform(Range, MaxSurfaceAngle, 2000.0f);

    UpdateIndicator(EffectTransform.GetLocation());
}

void APlayerTeleportAbility::SpawnTeleportParticle()
{
    UGameplayStatics::SpawnEmitterAttached(TeleportParticle, GetCharacter()->GetMesh());
}

void APlayerTeleportAbility::PlaySound()
{
    UGameplayStatics::SpawnSoundAttached(Sound, GetCharacter()->GetMesh());
}

bool APlayerTeleportAbility::AbilityTeleport()
{
    float Z = GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    FVector ZVector(0.0f, 0.0f, Z);
    FVector DestLoc = ZVector + EffectTransform.GetLocation();
    FRotator DestRot(
        GetOwner()->GetActorRotation().Pitch,
        EffectTransform.GetRotation().Rotator().Yaw,
        GetOwner()->GetActorRotation().Roll);

    return GetOwner()->TeleportTo(DestLoc, DestRot);
}
