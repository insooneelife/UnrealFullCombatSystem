// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetGuidLibrary.h"
#include "GameFramework/Character.h"
#include "GameCore/DCSGameMode.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GamePlay/PickupActor.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    SpawnPickupActorClass =
        GameUtils::LoadAssetClass<APickupActor>(TEXT("/Game/DynamicCombatSystem/Blueprints/BP_PickupActor"));
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    ClearInventory();

    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (GameMode != nullptr)
        {
            GameMode->OnGameLoaded.AddDynamic(this, &UInventoryComponent::OnGameLoaded);
        }
    }
}

void UInventoryComponent::OnGameLoaded()
{
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameMode != nullptr)
    {
        Inventory = GameMode->GetInventory();
        ClearInventory();
    }
}

void UInventoryComponent::AddItem(TSubclassOf<UItemBase> InItemClass, int InAmount)
{
    if (UKismetSystemLibrary::IsValidClass(InItemClass) && InAmount > 0)
    {
        UItemBase* ItemBase = Cast<UItemBase>(InItemClass->GetDefaultObject());
        if(ItemBase->GetItem().bIsStackable)
        {
            int Index = FindIndexByClass(InItemClass);

            if (Index >= 0)
            {
                Inventory[Index].Amount += InAmount;
                OnItemAdded.Broadcast(Inventory[Index]);
            }
            else
            {
                FStoredItem NewItem;
                NewItem.Id = UKismetGuidLibrary::NewGuid();
                NewItem.ItemClass = InItemClass;
                NewItem.Amount = InAmount;
                Inventory.Add(NewItem);

                OnItemAdded.Broadcast(NewItem);
            }
        }
        else
        {
            if (InAmount > 1)
            {
                UE_LOG(LogTemp, Warning, TEXT("Tried to add more than 1 unstuckable item : %s"), *InItemClass->GetName());
            }

            FStoredItem NewItem;
            NewItem.Id = UKismetGuidLibrary::NewGuid();
            NewItem.ItemClass = InItemClass;
            NewItem.Amount = 1;
            Inventory.Add(NewItem);

            OnItemAdded.Broadcast(NewItem);
        }
    }
}

void UInventoryComponent::RemoveItem(TSubclassOf<UItemBase> InItemClass, int InAmount)
{
    if (InAmount > 0)
    {
        UItemBase* ItemBase = Cast<UItemBase>(InItemClass->GetDefaultObject());
        int Index = FindIndexByClass(InItemClass);

        if (ItemBase->GetItem().bIsStackable)
        {


            if (Index >= 0)
            {
                Inventory[Index].Amount -= InAmount;
                ClearInventory();
            }
        }
        else
        {
            if (Index >= 0)
            {
                for (int i = 1; i < InAmount; ++i)
                {
                    Inventory[i] = FStoredItem();
                }
            }
            
            ClearInventory();
        }
    }
}


void UInventoryComponent::ClearInventory()
{
    for (int i = Inventory.Num() - 1; i >= 0; --i)
    {
        if (IsSlotEmpty(i))
        {
            Inventory.RemoveAt(i);
        }
        else
        {
            UItemBase* ItemBase = Cast<UItemBase>(Inventory[i].ItemClass->GetDefaultObject());
            if (ItemBase != nullptr)
            {
                if (!ItemBase->GetItem().bIsStackable)
                {
                    Inventory[i].Amount = 1;
                }
            }
        }
    }
}

void UInventoryComponent::RemoveItemAtIndex(int Index, int InAmount)
{
    if (InAmount > 0)
    {
        if (!IsSlotEmpty(Index))
        {
            Inventory[Index].Amount -= InAmount;

            ClearInventory();

            OnItemRemoved.Broadcast(Inventory[Index]);
        }
    }
}


void UInventoryComponent::DropItem(FStoredItem InItem)
{
    int Index = FindIndexById(InItem.Id);

    TArray<AActor*> PickupActors;

    RemoveItemAtIndex(Index, InItem.Amount);

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupActor::StaticClass(), Actors);

    for (AActor* Actor : Actors)
    {
        const float PickupActorSearchRange = 300.0f;

        if (GetOwner()->GetDistanceTo(Actor) <= PickupActorSearchRange)
        {
            PickupActors.Add(Actor);
        }
    }

    AActor* ClosestActor = UDefaultGameInstance::GetClosestActor(GetOwner(), PickupActors);

    APickupActor* ClosestPickupActor = Cast<APickupActor>(ClosestActor);
    if (ClosestPickupActor->IsValidLowLevel())
    {
        ClosestPickupActor->AddItem(InItem.ItemClass, InItem.Amount);
    }
    else
    {
        FVector Loc = GetOwner()->GetActorLocation();
        FVector Forward = GetOwner()->GetActorForwardVector() * 150.0f;
        FVector Up = GetOwner()->GetActorUpVector() * (-250.0f);

        FVector Start = Loc + Forward;
        FVector End = Loc + Forward + Up;

        ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

        TArray<AActor*> IgnoreActors { GetOwner() };

        FHitResult HitResult;
        
        FVector SpawnLoc = Start;
        
        if (UKismetSystemLibrary::LineTraceSingle(
            GetWorld(), Start, End, Channel, false, IgnoreActors, EDrawDebugTrace::Type::None, HitResult, true))
        {
            SpawnLoc = HitResult.Location;
        }

        FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        APickupActor* SpawnedActor =
            GetWorld()->SpawnActor<APickupActor>(
                SpawnPickupActorClass, SpawnLoc, FRotator::ZeroRotator, SpawnParameters);

        if (SpawnedActor != nullptr)
        {
            SpawnedActor->AddItem(InItem.ItemClass, InItem.Amount);
        }
    }
    
}

void UInventoryComponent::UseItem(FGuid ItemId)
{
    int Index = FindIndexById(ItemId);

    if (IsSlotEmpty(Index))
    {


        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Inventory[Index].ItemClass);

        if (ItemBase != nullptr)
        {
            ItemBase->UseItem(GetOwner());


            UItemBase* DefaultItem = Cast<UItemBase>(Inventory[Index].ItemClass->GetDefaultObject());

            if (DefaultItem->GetItem().bIsConsumable)
            {
                RemoveItemAtIndex(Index, 1);
            }

        }
    }

}

FStoredItem UInventoryComponent::GetItemAtIndex(int Index) const
{
    if (IsSlotEmpty(Index))
    {
        return FStoredItem();
    }
    else
    {
        return Inventory[Index];
    }
}


int UInventoryComponent::FindIndexByClass(TSubclassOf<UItemBase> InItemClass) const
{
    for (int i = 0; i < Inventory.Num(); ++i)
    {
        if (Inventory[i].ItemClass == InItemClass && Inventory[i].Amount >= 1)
        {
            return i;
        }
    }

    return -1;
}


int UInventoryComponent::FindIndexById(FGuid Id) const
{
    for (int i = 0; i < Inventory.Num(); ++i)
    {
        if (Inventory[i].Id == Id && Inventory[i].Amount >= 1)
        {
            return i;
        }
    }

    return -1;
}


bool UInventoryComponent::IsSlotEmpty(int Index) const
{
    if (Inventory.IsValidIndex(Index))
    {
        if (IsItemValid(Inventory[Index]))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}

bool UInventoryComponent::IsItemValid(FStoredItem Item) const
{
    if (Item.Id.IsValid() && UKismetSystemLibrary::IsValidClass(Item.ItemClass) && Item.Amount > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}