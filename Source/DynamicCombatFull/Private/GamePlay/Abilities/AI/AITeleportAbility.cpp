// Fill out your copyright notice in the Description page of Project Settings.


#include "AITeleportAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"
#include "Particles/ParticleSystem.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Components/AbilityComponent.h"
#include "GamePlay/AI/MageAICharacter.h"

AAITeleportAbility::AAITeleportAbility()
{
    ManaCost = 15.0f;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_Teleport");
    AbilityMontages.Add(AnimMontage);


    static UEnvQuery* QueryObject = GameUtils::LoadAssetObject<UEnvQuery>(
        "/Game/DynamicCombatSystem/Blueprints/AI/EQS/EQS_TeleportSpot2");
    Query = QueryObject;
    
    static USoundBase* SoundObject = GameUtils::LoadAssetObject<USoundBase>(
        "/Game/DynamicCombatSystem/SFX/CUE/CUE_Teleport");
    Sound = SoundObject;

    static UParticleSystem* ParticleObject = GameUtils::LoadAssetObject<UParticleSystem>(
        "/Game/DynamicCombatSystem/VFX/P_Teleport");
    TeleportParticle = ParticleObject;
}

void AAITeleportAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        if (Query != nullptr)
        {
            UEnvQueryInstanceBlueprintWrapper* QueryInstance =
                UEnvQueryManager::RunEQSQuery(
                    GetWorld(), Query, GetOwner(), EEnvQueryRunMode::RandomBest25Pct, nullptr);

            if (GameUtils::IsValid(QueryInstance))
            {
                QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
                    this, &AAITeleportAbility::OnQueryFinished);
            }
        }
    }
}

void AAITeleportAbility::Effect()
{
    if (AbilityTeleport())
    {
        SpawnTeleportParticle();
        PlaySound();
        MakeOwnerUntargetable(0.25f);
        ConsumeMana();
    }

}

void AAITeleportAbility::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    TArray<FVector> ResultLocations;
    if (QueryInstance->GetQueryResultsAsLocations(ResultLocations))
    {
        FVector StartLocation = ResultLocations[0];
        FVector GroundLocation;
        FVector Normal;

        if (FindGroundLocation(StartLocation, 80.0f, GroundLocation, Normal))
        {
            FVector From = GetOwner()->GetActorLocation();
            FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(From, GroundLocation);

            FRotator Rot = UKismetMathLibrary::Conv_VectorToRotator(UnitVector);
            FTransform NewEffectTransform(Rot, GroundLocation);

            EffectTransform = NewEffectTransform;
            PlayAbilityMontage(GetAbilityMontage(0), NAME_None, true, 1.0f);
        }
        else
        {
            AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
        }
    }
    else
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
    }
}

void AAITeleportAbility::SpawnTeleportParticle()
{
    UGameplayStatics::SpawnEmitterAttached(
        TeleportParticle, 
        GetCharacter()->GetMesh(), 
        FName("spine_01"),
        FVector(0.0f, 0.0f, -50.0f),
        FRotator::ZeroRotator,
        EAttachLocation::Type::KeepRelativeOffset);
}

void AAITeleportAbility::MakeOwnerUntargetable(float InDuration)
{
    AMageAICharacter* MageCharacter = Cast<AMageAICharacter>(GetOwner());
    if (GameUtils::IsValid(MageCharacter))
    {
        MageCharacter->MakeUntargetable(InDuration);
    }
}

void AAITeleportAbility::PlaySound()
{
    UGameplayStatics::SpawnSoundAttached(Sound, GetCharacter()->GetMesh());
}

bool AAITeleportAbility::AbilityTeleport()
{
    FRotator OwnerRot = GetOwner()->GetActorRotation();
    FVector Start = GetEffectLocation();
    FVector Target = GetTargetLoc();

    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
    FRotator DestRot(OwnerRot.Pitch, LookAtRot.Yaw, OwnerRot.Roll);

    return GetOwner()->TeleportTo(GetEffectLocation(), DestRot, false, false);
}
