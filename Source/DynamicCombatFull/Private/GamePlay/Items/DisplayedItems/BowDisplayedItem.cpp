// Fill out your copyright notice in the Description page of Project Settings.


#include "BowDisplayedItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/EquipmentComponent.h"
#include "GamePlay/Items/ObjectItems/ArrowItem.h"
#include "Interfaces/IsArcher.h"

ABowDisplayedItem::ABowDisplayedItem()
{
    ArrowSocketName = FName("arrow");
    HandAttachmentSocket = FName("bow_use");
    AttachmentSocket = FName("bow");

    BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BowMesh");
    ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArrowMesh");
    ArrowMesh->AttachToComponent(BowMesh, FAttachmentTransformRules::KeepRelativeTransform, ArrowSocketName);
}

void ABowDisplayedItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    EquipmentComponent->OnActiveItemChanged.RemoveDynamic(this, &ABowDisplayedItem::OnActiveItemChanged);

    Super::EndPlay(EndPlayReason);
}

void ABowDisplayedItem::NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    Super::NativeInit(InEquipmentComponent, InType, InSlotIndex);
    UpdateArrowMesh();
    EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &ABowDisplayedItem::OnActiveItemChanged);
}

void ABowDisplayedItem::SimulatePhysics()
{
    Super::SimulatePhysics();

    ArrowMesh->SetCollisionProfileName(TEXT("Ragdoll"));
    ArrowMesh->SetSimulatePhysics(true);
}

bool ABowDisplayedItem::Attach()
{
    ECombatType CombatType = EquipmentComponent->GetCombatType();

    if (EquipmentComponent->IsInCombat() && CombatType == ECombatType::Range)
    {
        UpdateArrowVisibility(true);
    }
    else
    {
        UpdateArrowVisibility(true);
    }

    UpdateArrowAttachment(false);

    return Super::Attach();
}

UPrimitiveComponent* ABowDisplayedItem::GetPrimaryComponent() const
{
    return BowMesh;
}

void ABowDisplayedItem::OnActiveItemChanged(
    FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex)
{
    if (InType == EItemType::Arrows && 
        InNewItem.Id != LastUpdatedArrow.Id)
    {
        UpdateArrowMesh();
    }
}

void ABowDisplayedItem::UpdateArrowAttachment(bool bAttachToOwner)
{
    if (bAttachToOwner)
    {
        IIsArcher* IsArcher = Cast<IIsArcher>(GetOwner());
        ACharacter* Character = Cast<ACharacter>(GetOwner());

        if (IsArcher != nullptr && Character != nullptr)
        {
            FName SocketName = IsArcher->GetBowStringSocketName();

            FAttachmentTransformRules Rules(
                EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

            ArrowMesh->AttachToComponent(Character->GetMesh(), Rules, SocketName);
        }
    }
    else
    {
        FName SocketName = ArrowSocketName;

        FAttachmentTransformRules Rules(
            EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

        ArrowMesh->AttachToComponent(BowMesh, Rules, SocketName);
    }

    
}

void ABowDisplayedItem::UpdateArrowVisibility(bool bVisible)
{
    ArrowMesh->SetVisibility(bVisible);
}

void ABowDisplayedItem::UpdateArrowMesh()
{
    FStoredItem Item = GetEquipmentComponent()->GetActiveItem(EItemType::Arrows, 0);

    UClass* Class = Item.ItemClass;
    UArrowItem* ArrowItem = Cast<UArrowItem>(Class);

    if (ArrowItem != nullptr)
    {
        UArrowItem* ArrowItemObject = NewObject<UArrowItem>(GetOwner(), Class);
        ArrowMesh->SetStaticMesh(ArrowItemObject->GetArrowMesh());
    }
    else
    {
        ArrowMesh->SetStaticMesh(nullptr);
    }
}
