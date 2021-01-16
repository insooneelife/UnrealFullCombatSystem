// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_GetBackstabDmg.h"
#include "Components/EffectsComponent.h"
#include "GameCore/GameUtils.h"
#include "GameCore/CustomStructs.h"
#include "GameCore/DefaultGameInstance.h"
#include "Interfaces/CanBeAttacked.h"


void UAN_GetBackstabDmg::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor* Owner = MeshComp->GetOwner();
    if (GameUtils::IsValid(Owner))
    {
        UEffectsComponent* EffectsComponent =
            Cast<UEffectsComponent>(Owner->GetComponentByClass(UEffectsComponent::StaticClass()));

        if (GameUtils::IsValid(EffectsComponent))
        {
            if (EffectsComponent->IsEffectApplied(EEffectType::Backstab))
            {
                ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(Owner);
                if (CanBeAttacked != nullptr)
                {
                    float Damage = EffectsComponent->GetBackstabDamage();
                    AActor* Applier = EffectsComponent->GetEffectApplier(EEffectType::Backstab);

                    FHitData HitData;
                    HitData.Damage = Damage;
                    HitData.DamageCauser = Applier;
                    HitData.HitFromDirection = FVector::ZeroVector;
                    HitData.bCanBeParried = false;
                    HitData.bCanBeBlocked = false;
                    HitData.bCanReceiveImpact = false;

                    EAttackResult AttackResult;
                    if (CanBeAttacked->TakeDamage(HitData, AttackResult))
                    {
                        UDefaultGameInstance* DefaultGameInstance =
                            Cast<UDefaultGameInstance>(GetWorld()->GetGameInstance());

                        if (GameUtils::IsValid(DefaultGameInstance))
                        {
                            DefaultGameInstance->PlayHitSound(Applier, Owner, Owner->GetActorLocation());
                        }
                    }
                }
            }
        }
    }
}