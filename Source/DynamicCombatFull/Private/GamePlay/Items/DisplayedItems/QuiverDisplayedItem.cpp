// Fill out your copyright notice in the Description page of Project Settings.


#include "QuiverDisplayedItem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/EquipmentComponent.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Items/ObjectItems/ArrowItem.h"

AQuiverDisplayedItem::AQuiverDisplayedItem()
{
    AttachmentSocket = FName("quiver");

    RootComponent = StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->ComponentTags.Add("Dissolve");
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    InstancedArrowsMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedArrowsMesh");
    InstancedArrowsMesh->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
    InstancedArrowsMesh->ComponentTags.Add("Dissolve");
    InstancedArrowsMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
    InstancedArrowsMesh->InstancingRandomSeed = 26425;

    TArray<FVector> ArrowSpawnLocations =
    {
        FVector(0.0f, 0.0f, 53.0f),
        FVector(-2.0f, -4.0f, 53.0f),
        FVector(-2.0f, 6.0f, 53.0f),
        FVector(2.0f, -5.0f, 53.0f),
        FVector(-3.0f, 0.0f, 53.0f),
        FVector(0.0f, -4.0f, 53.0f),
        FVector(0.0f, 5.0f, 53.0f),
        FVector(2.0f, 4.0f, 53.0f),
        FVector(3.0f, 2.0f, 53.0f)
    };

    TArray<FRotator> ArrowSpawnRotations =
    {
        FRotator(-90.0f, 0.0f, 0.0f),
        FRotator(-90.0f, 90.0f, -90.0f),
        FRotator(-90.0f, -90.0f, 90.0f),
        FRotator(-90.0f, 90.0f, -90.0f),
        FRotator(-90.0f, 0.0f, 0.0f),
        FRotator(-90.0f, 90.0f, -90.0f),
        FRotator(-90.0f, -90.0f, 90.0f),
        FRotator(-90.0f, -90.0f, 90.0f),
        FRotator(-90.0f, -90.0f, 90.0f),
    };

    for (int i = 0; i < 9; ++i)
    {
        FString Name = FString("ArrowSpawn") + FString::FromInt(i);

        USceneComponent* ArrowSpawn = CreateDefaultSubobject<USceneComponent>(*Name);
        ArrowSpawn->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
        ArrowSpawn->SetRelativeLocationAndRotation(ArrowSpawnLocations[i], ArrowSpawnRotations[i]);
        ArrowSpawns.Add(ArrowSpawn);
    }
}

void AQuiverDisplayedItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    StaticMesh = nullptr;
    InstancedArrowsMesh = nullptr;
    ArrowSpawns.Empty();
}

void AQuiverDisplayedItem::UpdateQuiver()
{
    LastUpdatedArrow = EquipmentComponent->GetActiveItem(EItemType::Arrows, 0);

    InstancedArrowsMesh->ClearInstances();

    // Rebuild arrows instances (subtract 1 from amount because 1 arrows is always in hand)
    for (int i = 1; i < LastUpdatedArrow.Amount && i < ArrowSpawns.Num(); ++i)
    {
        InstancedArrowsMesh->AddInstance(ArrowSpawns[i]->GetRelativeTransform());
    }
}

void AQuiverDisplayedItem::NativeInit(UEquipmentComponent* InEquipmentComponent, EItemType InType, int InSlotIndex)
{
    Super::NativeInit(InEquipmentComponent, InType, InSlotIndex);
    UpdateArrowMesh();
    EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &AQuiverDisplayedItem::OnActiveItemChanged);
}

void AQuiverDisplayedItem::OnActiveItemChanged(
    const FStoredItem& InOldItem, 
    const FStoredItem& InNewItem,
    EItemType InType, 
    int InSlotIndex,
    int InActiveIndex)
{
    if (InType == EItemType::Arrows && InNewItem.Amount > LastUpdatedArrow.Amount)
    {
        UpdateQuiver();
    }
}

void AQuiverDisplayedItem::UpdateArrowMesh()
{
    FStoredItem Item = EquipmentComponent->GetActiveItem(EItemType::Arrows, 0);

    TSubclassOf<UItemBase> ItemClass = Item.ItemClass;
    UArrowItem* ArrowItem = NewObject<UArrowItem>(GetOwner(), ItemClass);

    InstancedArrowsMesh->SetStaticMesh(ArrowItem->GetArrowMesh());

}