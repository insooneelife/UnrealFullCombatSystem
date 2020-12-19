// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellDisplayedItem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/GameUtils.h"

ASpellDisplayedItem::ASpellDisplayedItem()
{
    static UParticleSystem* LoadedObject =
        GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_ArcaneHand"));

    HandParticle = LoadedObject;
}

void ASpellDisplayedItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    ParticleSystemComponent->SetTemplate(HandParticle);
}


bool ASpellDisplayedItem::Attach()
{
    if (EquipmentComponent->IsSlotHidden(Type, SlotIndex))
    {
        ParticleSystemComponent->Deactivate();
        return true;
    }
    else
    {
        if (EquipmentComponent->IsInCombat())
        {
            ParticleSystemComponent->Activate();
            return Super::Attach();
        }
        else
        {
            ParticleSystemComponent->Deactivate();
            return true;
        }
    }
}






