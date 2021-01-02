// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/InventoryComponent.h"
#include "GameCore/GameUtils.h"

// Sets default values
APickupActor::APickupActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    TSubclassOf<UUserWidget> LoadedClass = 
        GameUtils::LoadAssetClass<UUserWidget>(TEXT("/Game/DynamicCombatSystem/Widgets/PickupWB"));

    CreateUserWidgetClass = LoadedClass;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Scene");
    Box = CreateDefaultSubobject<UBoxComponent>("Box");
    Box->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
    ParticleSystem->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform);
}

void APickupActor::Init(FName InName, const TMap<TSubclassOf<UItemBase>, int>& InItems)
{
    Name = InName;
    Items = InItems;

    RemoveInvalidItems();
    DestroyActorIfEmpty();
}

void APickupActor::RemoveInvalidItems()
{
    TArray<TSubclassOf<UItemBase>> Keys;
    Items.GetKeys(Keys);

    TArray<int> Values;
    for (auto E : Items)
    {
        Values.Add(E.Value);
    }

    for (int i = Keys.Num() - 1; i >= 0; --i)
    {
        TSubclassOf<UItemBase> Key = Keys[i];
        int Value = Values[i];

        if (!UKismetSystemLibrary::IsValidClass(Key) || Value <= 0)
        {
            Items.Remove(Key);
        }
    }
}

void APickupActor::TakeItem(TSubclassOf<UItemBase> ItemClass)
{
    if (Items.Contains(ItemClass))
    {
        int Amount = Items[ItemClass];

        UItemBase* ItemBase = Cast<UItemBase>(ItemClass->GetDefaultObject());

        if (GameUtils::IsValid(ItemBase))
        {
            if (ItemBase->GetItem().bIsStackable)
            {
                InventoryComponent->AddItem(ItemClass, Amount);
            }
            else
            {
                for (int i = 0; i < Amount; ++i)
                {
                    InventoryComponent->AddItem(ItemClass, 1);
                }
            }
        }

        Items.Remove(ItemClass);
        DestroyActorIfEmpty();
    }
    else
    {

    }
}

void APickupActor::TakeAllItems()
{
    for (auto e : Items)
    {
        TSubclassOf<UItemBase> ItemClass = e.Key;
        int Amount = e.Value;

        UItemBase* ItemBase = Cast<UItemBase>(ItemClass->GetDefaultObject());

        if (GameUtils::IsValid(ItemBase))
        {
            if (ItemBase->GetItem().bIsStackable)
            {
                InventoryComponent->AddItem(ItemClass, Amount);
            }
            else
            {
                for (int i = 0; i < Amount; ++i)
                {
                    InventoryComponent->AddItem(ItemClass, 1);
                }
            }
        }

        Items.Remove(ItemClass);
        DestroyActorIfEmpty();
    }
}

void APickupActor::AddItem(TSubclassOf<UItemBase> ItemClass, int Amount)
{
    if (Items.Contains(ItemClass))
    {
        Items[ItemClass] += Amount;
    }
    else
    {
        Items.Add(ItemClass, Amount);
    }
}

void APickupActor::Interact(AActor* InCaller)
{
    UInventoryComponent* InventoryComp =
        Cast<UInventoryComponent>(InCaller->GetComponentByClass(UInventoryComponent::StaticClass()));

    if (GameUtils::IsValid(InventoryComp))
    {
        InventoryComponent = InventoryComp;

        APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        if (GameUtils::IsValid(PlayerCon))
        {
            // call blueprint function
            UUserWidget* Widget = CreatePickupWidget(PlayerCon, CreateUserWidgetClass, this);
            Widget->AddToViewport();
        }
    }
}

void APickupActor::DestroyActorIfEmpty()
{
    if (Items.Num() <= 0)
    {
        Destroy();
    }
}