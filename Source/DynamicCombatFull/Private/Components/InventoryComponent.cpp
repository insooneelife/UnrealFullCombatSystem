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
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    ClearInventory();

    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (GameUtils::IsValid(GameMode))
        {
            GameMode->OnGameLoaded.AddDynamic(this, &UInventoryComponent::OnGameLoaded);
        }
    }
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

        if (GameUtils::IsValid(GameMode))
        {
            GameMode->OnGameLoaded.RemoveDynamic(this, &UInventoryComponent::OnGameLoaded);
        }
    }

    Super::EndPlay(EndPlayReason);
}

void UInventoryComponent::UseItem(FGuid InItemId)
{
    int Index = FindIndexById(InItemId);

    if (!IsSlotEmpty(Index))
    {
        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), Inventory[Index].ItemClass);

        if (GameUtils::IsValid(ItemBase))
        {
            const FItem& UseItemData = ItemBase->GetItem();
            ItemBase->UseItem(GetOwner());

            if (UseItemData.bIsConsumable)
            {
                RemoveItemAtIndex(Index, 1);
            }
        }
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

int UInventoryComponent::FindIndexById(FGuid InItemId) const
{
    for (int i = 0; i < Inventory.Num(); ++i)
    {
        if (Inventory[i].Id == InItemId && Inventory[i].Amount >= 1)
        {
            return i;
        }
    }

    return -1;
}


FStoredItem UInventoryComponent::GetItemAtIndex(int InIndex) const
{
    if (IsSlotEmpty(InIndex))
    {
        return FStoredItem();
    }
    else
    {
        return Inventory[InIndex];
    }
}

void UInventoryComponent::RemoveItemAtIndex(int InIndex, int InAmount)
{
    if (InAmount > 0)
    {
        if (!IsSlotEmpty(InIndex))
        {
            Inventory[InIndex].Amount -= InAmount;

            FStoredItem SavedItem = Inventory[InIndex];

            ClearInventory();

            OnItemRemoved.Broadcast(SavedItem);
        }
    }
}

void UInventoryComponent::AddItem(TSubclassOf<UItemBase> InItemClass, int InAmount)
{
    if (UKismetSystemLibrary::IsValidClass(InItemClass) && InAmount > 0)
    {
        UItemBase* ItemBase = Cast<UItemBase>(InItemClass->GetDefaultObject());
        if (ItemBase->GetItem().bIsStackable)
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
                UE_LOG(LogTemp, Warning, TEXT("Tried to add more than 1 unstuckable item : %s"), 
                    *InItemClass->GetName());
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

void UInventoryComponent::DropItem(FStoredItem InItem)
{
    int Index = FindIndexById(InItem.Id);

    TArray<AActor*> PickupActors;

    RemoveItemAtIndex(Index, InItem.Amount);

    // If there is any Pickup Actor around owner, choose closer one and add dropped item to it, otherwise spawn new PickupActor
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

    if (GameUtils::IsValid(ClosestPickupActor))
    {
        ClosestPickupActor->AddItem(InItem.ItemClass, InItem.Amount);
    }
    else
    {
        // Find spot to spawn new pickup actor
        FVector Loc = GetOwner()->GetActorLocation();
        FVector Forward = GetOwner()->GetActorForwardVector() * 150.0f;
        FVector Up = GetOwner()->GetActorUpVector() * (-250.0f);

        FVector Start = Loc + Forward;
        FVector End = Loc + Forward + Up;

        ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
        TArray<AActor*> IgnoreActors{ GetOwner() };
        FHitResult HitResult;
        FVector SpawnLoc = Start;

        if (UKismetSystemLibrary::LineTraceSingle(
            GetWorld(), Start, End, Channel, false, IgnoreActors, EDrawDebugTrace::Type::None, HitResult, true))
        {
            SpawnLoc = HitResult.Location;
        }

        FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        UDefaultGameInstance* DefaultGameInstance = GameUtils::GetDefaultGameInstance(GetWorld());

        if (DefaultGameInstance != nullptr)
        {
            APickupActor* SpawnedActor =
                GetWorld()->SpawnActor<APickupActor>(
                    SpawnPickupActorClass, SpawnLoc, FRotator::ZeroRotator, SpawnParameters);

            if (GameUtils::IsValid(SpawnedActor))
            {
                SpawnedActor->Init(
                    FName(TEXT("Items")),
                    TMap<TSubclassOf<UItemBase>, int> { { InItem.ItemClass, InItem.Amount } }
                );
                SpawnedActor->AddItem(InItem.ItemClass, InItem.Amount);
            }
        }
    }
}


void UInventoryComponent::OnGameLoaded()
{
    ADCSGameMode* GameMode = Cast<ADCSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (GameUtils::IsValid(GameMode))
    {
        Inventory = GameMode->GetInventory();
        ClearInventory();
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
            const FItem& Item = GameUtils::GetDefaultItemFromStoredItem(Inventory[i]);
            
            if (!Item.bIsStackable)
            {
                Inventory[i].Amount = 1;
            }
        }
    }
}

bool UInventoryComponent::IsSlotEmpty(int InIndex) const
{
    if (Inventory.IsValidIndex(InIndex))
    {
        if (IsItemValid(Inventory[InIndex]))
        {
            return false;
        }
    }
    return true;
}

bool UInventoryComponent::IsItemValid(FStoredItem InItem) const
{
    if (InItem.Id.IsValid() && UKismetSystemLibrary::IsValidClass(InItem.ItemClass) && InItem.Amount > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}