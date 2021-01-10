// Fill out your copyright notice in the Description page of Project Settings.


#include "MontageAction.h"
#include "GameCore/GameUtils.h"
#include "Components/MontageManagerComponent.h"
#include "Components/EquipmentComponent.h"

// Add default functionality here for any IIMontageAction functions that are not pure virtual.
UAnimMontage* IMontageAction::GetStunMontage(EDirection Direction) const
{
    EMontageAction StunDirection;
    if (Direction == EDirection::Front)
    {
        StunDirection = EMontageAction::StunFront;
    }
    else if (Direction == EDirection::Back)
    {
        StunDirection = EMontageAction::StunBack;
    }
    else if (Direction == EDirection::Left)
    {
        StunDirection = EMontageAction::StunLeft;
    }
    else
    {
        StunDirection = EMontageAction::StunRight;
    }
    UMontageManagerComponent* MontageManager = GetMontageManager();

    if (!GameUtils::IsValid(MontageManager))
    {
        return nullptr;
    }

    int Index = MontageManager->GetRandomMontageIndex(StunDirection);
    UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(StunDirection, Index);

    if (AnimMontage != nullptr)
    {
        return AnimMontage;
    }
    else
    {
        if (StunDirection != EMontageAction::StunFront)
        {
            Index = MontageManager->GetRandomMontageIndex(EMontageAction::StunFront);
            return MontageManager->GetMontageForAction(EMontageAction::StunFront, Index);
        }
        else
        {
            return nullptr;
        }
    }
}

UAnimMontage* IMontageAction::GetBlockMontage() const
{
    UMontageManagerComponent* MontageManager = GetMontageManager();
    UEquipmentComponent* Equipment = GetEquipment();

    if (GameUtils::IsValid(MontageManager) && GameUtils::IsValid(Equipment))
    {
        int Index = Equipment->IsShieldEquipped() ? 1 : 0;
        return MontageManager->GetMontageForAction(EMontageAction::Block, Index);
    }
    return nullptr;
}

UAnimMontage* IMontageAction::GetImpactMontage() const
{
    UMontageManagerComponent* MontageManager = GetMontageManager();
    EMontageAction MontageActionType = EMontageAction::Impact;

    if (GameUtils::IsValid(MontageManager))
    {
        int Index = MontageManager->GetRandomMontageIndex(MontageActionType);
        return MontageManager->GetMontageForAction(MontageActionType, Index);
    }
    return nullptr;
}

UAnimMontage* IMontageAction::GetParriedMontage() const
{
    UMontageManagerComponent* MontageManager = GetMontageManager();
    EMontageAction MontageActionType = EMontageAction::Parried;

    if (GameUtils::IsValid(MontageManager))
    {
        int Index = MontageManager->GetRandomMontageIndex(MontageActionType);
        return MontageManager->GetMontageForAction(MontageActionType, Index);
    }
    return nullptr;
}

UAnimMontage* IMontageAction::GetParryMontage() const
{
    UMontageManagerComponent* MontageManager = GetMontageManager();
    UEquipmentComponent* Equipment = GetEquipment();

    if (GameUtils::IsValid(MontageManager) && GameUtils::IsValid(Equipment))
    {
        int Index = Equipment->IsShieldEquipped() ? 1 : 0;
        return MontageManager->GetMontageForAction(EMontageAction::Parry, Index);
    }
    return nullptr;
}