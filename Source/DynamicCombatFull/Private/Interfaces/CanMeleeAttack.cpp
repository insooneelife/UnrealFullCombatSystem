// Fill out your copyright notice in the Description page of Project Settings.


#include "CanMeleeAttack.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"
#include "Interfaces/CanBeAttacked.h"
#include "Components/EffectsComponent.h"
#include "Components/MontageManagerComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/EquipmentComponent.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"

// Add default functionality here for any ICanMeleeAttack functions that are not pure virtual.
FHitData ICanMeleeAttack::MakeMeleeHitData(AActor* HitActor)
{
    float Damage = GetMeleeDamage();
    EMeleeAttackType MeleeAttackType = GetMeleeAttackType();
    float ScaledDamage = UDefaultGameInstance::ScaleMeleeDamageByType(Damage, MeleeAttackType);

    FVector HitFromDirection = UKismetMathLibrary::GetDirectionUnitVector(
            GetThisCharacter()->GetActorLocation(), HitActor->GetActorLocation());

    bool CanBeParried = MeleeAttackType == EMeleeAttackType::Light;
    FHitData HitData(ScaledDamage, GetThisCharacter(), HitFromDirection, CanBeParried, true, true);

    return HitData;
}


void ICanMeleeAttack::ApplyHitImpulseToCharacter(AActor* HitActor, FVector HitNormal, float ImpulsePower)
{
    ACharacter* Character = Cast<ACharacter>(HitActor);

    if (GameUtils::IsValid(Character))
    {
        if (Character->GetMesh()->IsAnySimulatingPhysics())
        {
            FVector Impulse = HitNormal * -1.0f * ImpulsePower;
            Character->GetMesh()->AddImpulse(Impulse);
        }
    }
}

// on hit melee attack
void ICanMeleeAttack::OnHit(const FHitResult& InHit)
{
    ACharacter* ThisCharacter = GetThisCharacter();
    FVector HitLocation = InHit.Location;
    FVector HitNormal = InHit.Normal;
    AActor* HitActor = InHit.GetActor();
    ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(HitActor);

    float Damage = GetMeleeDamage();
    EMeleeAttackType MeleeAttackType = GetMeleeAttackType();

    FHitData HitData = MakeMeleeHitData(InHit.GetActor());

    if (CanBeAttacked != nullptr)
    {
        EAttackResult OutAttackResult;
        bool bTakeDamageResult = CanBeAttacked->TakeDamage(HitData, OutAttackResult);

        ApplyHitImpulseToCharacter(HitActor, HitNormal, 15000.0f);

        if (bTakeDamageResult)
        {
            UDefaultGameInstance* DefaultGameInstance = Cast<UDefaultGameInstance>(ThisCharacter->GetGameInstance());
            DefaultGameInstance->PlayHitSound(ThisCharacter, HitActor, HitLocation);

            HitActor->GetComponentByClass(UEffectsComponent::StaticClass());
            UEffectsComponent* EffectsComponent =
                Cast<UEffectsComponent>(HitActor->GetComponentByClass(UEffectsComponent::StaticClass()));

            if (GameUtils::IsValid(EffectsComponent))
            {
                EffectsComponent->ApplyEffect(EEffectType::Stun, 2.0f, EApplyEffectMethod::Replace, ThisCharacter);
            }
        }
    }
}

UAnimMontage* ICanMeleeAttack::GetNextMeleeAttackMontage(
    UMontageManagerComponent* MontageManager, EMeleeAttackType AttackType)
{
    int MeleeAttackCounter = GetMeleeAttackCounter();
    EMontageAction Action = UDefaultGameInstance::ConvertMeleeAttackTypeToAction(AttackType);
    int LastIndex = MontageManager->GetMontageActionLastIndex(Action);
    int Index = MeleeAttackCounter > LastIndex ? LastIndex : MeleeAttackCounter;
    UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(Action, Index);

    MeleeAttackCounter++;
    if (MeleeAttackCounter > LastIndex)
    {
        MeleeAttackCounter = 0;
    }

    SetMeleeAttackCounter(MeleeAttackCounter);

    return AnimMontage;
}


void ICanMeleeAttack::OnCollisionActivated(ECollisionPart CollisionPart)
{
    ACharacter* ThisCharacter = GetThisCharacter();
    UCollisionHandlerComponent* MeleeCollisionHandler = GetMeleeCollisionHandler();
    UEquipmentComponent* Equipment = GetEquipment();

    if (GameUtils::IsValid(MeleeCollisionHandler) &&
        GameUtils::IsValid(Equipment) &&
        GameUtils::IsValid(ThisCharacter))
    {
        if (CollisionPart == ECollisionPart::MainHandItem)
        {
            EItemType ItemType = Equipment->GetSelectedMainHandType();
            ADisplayedItem* DisplayedItem = Equipment->GetDisplayedItem(ItemType, 0);

            if (GameUtils::IsValid(DisplayedItem))
            {
                MeleeCollisionHandler->SetCollisionMesh(
                    DisplayedItem->GetPrimaryComponent(), DisplayedItem->GetPrimaryComponent()->GetAllSocketNames());

            }
        }
        else if (CollisionPart == ECollisionPart::RightHand)
        {
            MeleeCollisionHandler->SetCollisionMesh(ThisCharacter->GetMesh(), GetRightHandCollisionSockets());
        }
        else if (CollisionPart == ECollisionPart::LeftHand)
        {
            MeleeCollisionHandler->SetCollisionMesh(ThisCharacter->GetMesh(), GetLeftHandCollisionSockets());
        }
        else if (CollisionPart == ECollisionPart::RightFoot)
        {
            MeleeCollisionHandler->SetCollisionMesh(ThisCharacter->GetMesh(), GetRightFootCollisionSockets());
        }
        else if (CollisionPart == ECollisionPart::LeftFoot)
        {
            MeleeCollisionHandler->SetCollisionMesh(ThisCharacter->GetMesh(), GetLeftFootCollisionSockets());
        }
    }
}