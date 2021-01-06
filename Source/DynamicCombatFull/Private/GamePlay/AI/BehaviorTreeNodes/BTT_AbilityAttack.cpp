// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AbilityAttack.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Abilities/AIAbilityBase.h"
#include "Components/AbilityComponent.h"

UBTT_AbilityAttack::UBTT_AbilityAttack(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer)
{
    TSubclassOf<AAIAbilityBase> LoadedClass = GameUtils::LoadAssetClass<AAIAbilityBase>(
        "/Game/DynamicCombatSystem/Blueprints/Abilities/AI/AITeleportAbilityBP");

    AbilityClass = LoadedClass;
}

void UBTT_AbilityAttack::ReceiveExecuteAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    AbilityAttack(ControlledPawn);
}

void UBTT_AbilityAttack::ReceiveAbortAI(AAIController* OwnerController, APawn* ControlledPawn)
{
    UAbilityComponent* AbilityComponent = 
        Cast<UAbilityComponent>(ControlledPawn->GetComponentByClass(UAbilityComponent::StaticClass()));

    if (GameUtils::IsValid(AbilityComponent))
    {
        AbilityComponent->EndAbility(EAbilityEndResult::Interrupted);
    }

    FinishAbort();
}

void UBTT_AbilityAttack::OnAbilityEnded(EAbilityEndResult InResult)
{
    FinishExecute(true);
}

void UBTT_AbilityAttack::AbilityAttack(APawn* InControlledPawn)
{
    UAbilityComponent* AbilityComponent = 
        Cast<UAbilityComponent>(InControlledPawn->GetComponentByClass(UAbilityComponent::StaticClass()));

    if (GameUtils::IsValid(AbilityComponent))
    {
        AbilityComponent->UpdateAbility(AbilityClass);
        AbilityComponent->AbilityPressed();
        AbilityComponent->AbilityReleased();

        if (AbilityComponent->IsCasting())
        {
            AbilityComponent->OnAbilityEnded.AddDynamic(this, &UBTT_AbilityAttack::OnAbilityEnded);
        }
        else
        {
            FinishExecute(true);
        }
    }
    else
    {
        FinishExecute(true);
    }
}
