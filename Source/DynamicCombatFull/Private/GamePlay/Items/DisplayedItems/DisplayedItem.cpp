// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayedItem.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ADisplayedItem::ADisplayedItem()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>("Scene");

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
}

void ADisplayedItem::NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    EquipmentComponent = InEquipmentComponent;
    Type = InType;
    SlotIndex = InSlotIndex;

    if (GetPrimaryComponent()->IsValidLowLevel())
    {
        GetPrimaryComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
    }
}

void ADisplayedItem::Init(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    NativeInit(InEquipmentComponent, InType, InSlotIndex);
    K2_Init();
}


UPrimitiveComponent* ADisplayedItem::GetPrimaryComponent() const
{
    {
        UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(
            GetComponentByClass(UStaticMeshComponent::StaticClass()));

        if (Comp->IsValidLowLevel())
        {
            return Comp;
        }
    }
    
    {
        USkeletalMeshComponent* Comp = Cast<USkeletalMeshComponent>(
            GetComponentByClass(USkeletalMeshComponent::StaticClass()));

        if (Comp->IsValidLowLevel())
        {
            return Comp;
        }
    }
    
    {
        UParticleSystemComponent* Comp = Cast<UParticleSystemComponent>(
            GetComponentByClass(UParticleSystemComponent::StaticClass()));

        if (Comp->IsValidLowLevel())
        {
            return Comp;
        }
    }

    return nullptr;
}

bool ADisplayedItem::Attach()
{
    if (GetPrimaryComponent()->IsValidLowLevel())
    {
        ACharacter* Character = Cast<ACharacter>(GetOwner());

        if (Character->IsValidLowLevel())
        {
            FAttachmentTransformRules AttachmentRules(
                EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

            GetPrimaryComponent()->AttachToComponent(Character->GetMesh(), AttachmentRules, GetAttachmentSocket());
            return true;
        }
    }
    return false;
}

void ADisplayedItem::SimulatePhysics()
{
    GetPrimaryComponent()->SetCollisionProfileName(FName(TEXT("Ragdoll")), true);
    GetPrimaryComponent()->SetSimulatePhysics(true);
}



