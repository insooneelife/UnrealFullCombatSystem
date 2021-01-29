// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayedItem.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameCore/GameUtils.h"

// Sets default values
ADisplayedItem::ADisplayedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
}

void ADisplayedItem::NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    EquipmentComponent = InEquipmentComponent;
    Type = InType;
    SlotIndex = InSlotIndex;

    TArray<UActorComponent*> Components;
    GetComponents(UPrimitiveComponent::StaticClass(), Components);

    for (UActorComponent* Component : Components)
    {
        UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Component);
        
        if (GameUtils::IsValid(PrimitiveComp))
        {
            PrimitiveComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
        }
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

        if (Comp != nullptr)
        {
            return Comp;
        }
    }
    
    {
        USkeletalMeshComponent* Comp = Cast<USkeletalMeshComponent>(
            GetComponentByClass(USkeletalMeshComponent::StaticClass()));

        if (Comp != nullptr)
        {
            return Comp;
        }
    }
    
    {
        UParticleSystemComponent* Comp = Cast<UParticleSystemComponent>(
            GetComponentByClass(UParticleSystemComponent::StaticClass()));

        if (Comp != nullptr)
        {
            return Comp;
        }
    }

    return nullptr;
}

bool ADisplayedItem::Attach()
{
    if (GameUtils::IsValid(GetPrimaryComponent()))
    {
        ACharacter* Character = Cast<ACharacter>(GetOwner());

        if (GameUtils::IsValid(Character))
        {
            UPrimitiveComponent* PrimitiveComponent = GetPrimaryComponent();

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



