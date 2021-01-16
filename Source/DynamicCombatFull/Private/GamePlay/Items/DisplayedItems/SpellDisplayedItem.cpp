// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellDisplayedItem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/GameUtils.h"

ASpellDisplayedItem::ASpellDisplayedItem()
{
    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
    
    static UParticleSystem* LoadedObject =
        GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_ArcaneHand"));

    ParticleSystemComponent->SetTemplate(LoadedObject);
    ParticleSystemComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    HandAttachmentSocket = FName("magic_right_hand");
    AttachmentSocket = FName("magic_right_hand");
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






