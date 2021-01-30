// Fill out your copyright notice in the Description page of Project Settings.


#include "AISummonMinionAbility.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "Materials/MaterialInstance.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"
#include "GamePlay/AbilityEffects/DamageSphereAbilityEffect.h"
#include "GamePlay/AI/AICharacter.h"
#include "GamePlay/AI/BaseAIController.h"
#include "Components/AbilityComponent.h"

AAISummonMinionAbility::AAISummonMinionAbility()
{
    ManaCost = 40.0f;

    static UAnimMontage* AnimMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/AI/Magic/M_AI_Magic_SummonMinion");
    AbilityMontages.Add(AnimMontage);


    static UEnvQuery* Query = GameUtils::LoadAssetObject<UEnvQuery>(
        "/Game/DynamicCombatSystem/Blueprints/AI/EQS/EQS_SummonSpot2");
    QueryObject = Query;

    static USoundBase* Sound = GameUtils::LoadAssetObject<USoundBase>(
        "/Game/DynamicCombatSystem/SFX/CUE/CUE_MinionSpawn");
    SoundObject = Sound;
}

void AAISummonMinionAbility::Released()
{
    if (AbilityComponent->StartAbility())
    {
        if (QueryObject != nullptr)
        {
            UEnvQueryInstanceBlueprintWrapper* QueryInstance =
                UEnvQueryManager::RunEQSQuery(
                    GetWorld(), QueryObject, GetOwner(), EEnvQueryRunMode::RandomBest25Pct, nullptr);

            if (GameUtils::IsValid(QueryInstance))
            {
                QueryInstance->GetOnQueryFinishedEvent().AddDynamic(
                    this, &AAISummonMinionAbility::OnQueryFinished);
            }
        }
    }
}

void AAISummonMinionAbility::Effect()
{
    SummonMinion();
    PlaySound();
    ConsumeMana();
}

void AAISummonMinionAbility::OnQueryFinished(
    UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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

void AAISummonMinionAbility::PlaySound()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundObject, GetEffectLocation());
}

void AAISummonMinionAbility::SummonMinion()
{
    FVector Start = GetEffectLocation();
    FVector Target = GetTargetLoc();

    FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
    FRotator SpawnRot(0.0f, Rot.Yaw, 0.0f);

    AAICharacter* SpawnedAI = Cast<AAICharacter>(UAIBlueprintHelperLibrary::SpawnAIFromClass(
        GetWorld(), SpawnedMinionClass, nullptr, Start, SpawnRot, true, GetOwner()));

    if (GameUtils::IsValid(SpawnedAI))
    {
        SpawnedAI->GetAIController()->SetTarget(GetTarget());
    }
}
