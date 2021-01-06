// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "GamePlay/AI/BaseAIController.h"
#include "GameCore/GameUtils.h"

#include "Components/StatsManagerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/ExtendedStatComponent.h"
#include "Components/EffectsComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

    StatsManager->Init();
    Equipment->Init();
    InitializeStatsWidget();

    ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetController());

    if (GameUtils::IsValid(BaseAIController))
    {
        AIController = BaseAIController;
    }

    float Health = ExtendedHealth->GetMaxValue();
    ExtendedHealth->SetCurrentValue(Health, false);
}

void AAICharacter::OnEffectApplied(EEffectType InType)
{
    if (InType == EEffectType::Stun)
    {
        Stunned();
    }
    else if (InType == EEffectType::Backstab)
    {
        Backstabbed();
    }
    else if (InType == EEffectType::Impact)
    {
        Impact();
    }
    else if (InType == EEffectType::Parried)
    {
        Parried();
    }

    StateManager->SetState(EState::Disabled);

}

void AAICharacter::OnEffectRemoved(EEffectType InType)
{
    if (InType == EEffectType::Stun ||
        InType == EEffectType::Knockdown ||
        InType == EEffectType::Impact ||
        InType == EEffectType::Parried ||
        InType == EEffectType::Backstab)
    {
        TArray<EEffectType> Array 
        {
            EEffectType::Stun,
            EEffectType::Knockdown,
            EEffectType::Impact,
            EEffectType::Parried,
            EEffectType::Backstab
        };

        if (!Effects->IsAnyEffectApplied(Array))
        {
            StateManager->ResetState(0.0f);
        }
    }
}

void AAICharacter::OnCollisionActivated(ECollisionPart InCollisionPart)
{
}

void AAICharacter::HandleMeshOnDeath()
{
}

void AAICharacter::OnValueChanged_ExtendedHealth(float InNewValue, float InMaxValue)
{
}

void AAICharacter::InitializeStatsWidget()
{
}

UAnimMontage* AAICharacter::GetStunMontage(EDirection InDirection) const
{
    return nullptr;
}

UAnimMontage* AAICharacter::GetBlockMontage() const
{
    return nullptr;
}

UAnimMontage* AAICharacter::GetImpactMontage() const
{
    return nullptr;
}

UAnimMontage* AAICharacter::GetParriedMontage() const
{
    return nullptr;
}

UAnimMontage* AAICharacter::GetRollMontage() const
{
    return nullptr;
}

bool AAICharacter::CanBeStunned() const
{
    return false;
}

bool AAICharacter::CanBeAttacked() const
{
    return false;
}

bool AAICharacter::CanBeBackstabbed() const
{
    return false;
}

void AAICharacter::ShowStatsWidget()
{
}

void AAICharacter::HideStatsWidget()
{
}

void AAICharacter::Death()
{
}

void AAICharacter::Stunned()
{
}

void AAICharacter::Block()
{
}

void AAICharacter::Parried()
{
}

void AAICharacter::Impact()
{
}

void AAICharacter::Backstabbed()
{
}


float AAICharacter::Roll(EDirection InDirection)
{
    return 0.0f;
}

bool AAICharacter::IsStateEqualPure(EState InState) const
{
    return false;
}

bool AAICharacter::IsActivityPure(EActivity InActivity) const
{
    return false;
}

bool AAICharacter::IsCombatTypePure(ECombatType InType) const
{
    return false;
}

void AAICharacter::UpdateReceivedHitDirection(float InHitFromDirection)
{
}

bool AAICharacter::CanBeInterrupted() const
{
    return false;
}

void AAICharacter::OnSelected()
{
}

void AAICharacter::OnDeselected()
{
}

bool AAICharacter::IsTargetable() const
{
    return false;
}

bool AAICharacter::TakeDamage(const FHitData& InHitData, EAttackResult& OutResult)
{
    return false;
}

bool AAICharacter::IsAlive() const
{
    return false;
}

FName AAICharacter::GetHeadSocket() const
{
    return FName();
}

bool AAICharacter::CanEffectBeApplied(EEffectType Type, AActor* Applier)
{
    return false;
}

FRotator AAICharacter::GetDesiredRotation() const
{
    return FRotator();
}

UDataTable* AAICharacter::GetMontages(EMontageAction InAction) const
{
    return nullptr;
}


float AAICharacter::GetMeleeDamage() const
{
    return 0.0f;
}

float AAICharacter::MeleeAttack(EMeleeAttackType InType)
{
    return 0.0f;
}
