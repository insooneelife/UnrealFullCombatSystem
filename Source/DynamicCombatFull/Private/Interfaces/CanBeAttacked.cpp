// Fill out your copyright notice in the Description page of Project Settings.


#include "CanBeAttacked.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"
#include "GameCore/DefaultGameInstance.h"

#include "Components/EffectsComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/EquipmentComponent.h"


bool ICanBeAttacked::TakeDamage(const FHitData& HitData, EAttackResult& OutResult)
{
    ACharacter* ThisCharacter = GetThisCharacter();
    FVector ActorLocation = ThisCharacter->GetActorLocation();
    if (CanBeAttacked())
    {
        UpdateReceivedHitDirection(HitData.HitFromDirection);

        UDefaultGameInstance* DefaultGameInstance = Cast<UDefaultGameInstance>(ThisCharacter->GetGameInstance());

        if (HitData.Damage != 0.0f)
        {
            ReportDamage(HitData);

            UStateManagerComponent* StateManager = GetStateManager();

            if (!GameUtils::IsValid(StateManager))
            {
                return false;
            }

            // If hit was successfully Parried, apply Parried effect on attacker and don't subtract health
            bool bParried =
                GetReceivedHitDirection() == EDirection::Front &&
                StateManager->GetActivityValue(EActivity::CanParryHit) &&
                HitData.bCanBeParried;

            if (bParried)
            {
                UEffectsComponent* EffectsComponent = Cast<UEffectsComponent>(
                    HitData.DamageCauser->GetComponentByClass(UEffectsComponent::StaticClass()));

                if (GameUtils::IsValid(EffectsComponent))
                {
                    bool bApplied = EffectsComponent->ApplyEffect(
                        EEffectType::Parried, 1.0f, EApplyEffectMethod::Replace, ThisCharacter);

                    if (bApplied)
                    {
                        DefaultGameInstance->PlayParrySound(ThisCharacter, HitData.DamageCauser, ActorLocation);
                    }
                }

                OutResult = EAttackResult::Parried;
                return false;
            }

            // Check if hit was successfully blocked
            bool bBlocked = IsBlocked() && HitData.bCanBeBlocked;

            UStatsManagerComponent* StatsManager = GetStatsManager();
            if (GameUtils::IsValid(StatsManager))
            {
                StatsManager->TakeDamage(HitData.Damage, !bBlocked);
            }

            if (IsAlive() && bBlocked)
            {
                DefaultGameInstance->PlayBlockSound(ThisCharacter, HitData.DamageCauser, ActorLocation);
                Block();

                UEquipmentComponent* Equipment = GetEquipment();
                UExtendedStatComponent* ExtendedStamina = GetExtendedStaminaComp();

                if (GameUtils::IsValid(Equipment) && GameUtils::IsValid(ExtendedStamina))
                {
                    if (ExtendedStamina->GetCurrentValue() > 0.0f)
                    {
                        if (HitData.bCanReceiveImpact &&
                            (Equipment->IsShieldEquipped() || !(Equipment->GetCombatType() == ECombatType::Unarmed)))
                        {
                            UEffectsComponent* EffectsComponent = Cast<UEffectsComponent>(
                                HitData.DamageCauser->GetComponentByClass(UEffectsComponent::StaticClass()));

                            if (GameUtils::IsValid(EffectsComponent))
                            {
                                EffectsComponent->ApplyEffect(
                                    EEffectType::Impact, 1.0f, EApplyEffectMethod::Replace, ThisCharacter);
                            }
                        }
                    }
                }

                OutResult = EAttackResult::Blocked;
                return false;
            }


            OutResult = EAttackResult::Success;
            return true;
        }
    }

    OutResult = EAttackResult::Failed;
    return false;
}