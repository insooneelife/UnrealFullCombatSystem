// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/CustomStructs.h"
#include "Items/ObjectItems/ItemBase.h"
#include "Components/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/IsInteractable.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "PickupActor.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

UCLASS(BlueprintType, Blueprintable)
class APickupActor : public AActor, public IIsInteractable
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APickupActor();

public:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

public:
    void Init(FName InName, const TMap<TSubclassOf<UItemBase>, int>& InItems);
    void RemoveInvalidItems();
    void TakeItem(TSubclassOf<UItemBase> ItemClass);
    void TakeAllItems();
    void AddItem(TSubclassOf<UItemBase> ItemClass, int Amount);

    // blueprint implementation
public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    UUserWidget* CreatePickupWidget(AActor* InOwner, TSubclassOf<UUserWidget> WidgetClass, APickupActor* PickupActor);

    // IsInteractable
public:
    virtual void Interact(AActor* InCaller) override;
    virtual FName GetInteractionMessage() const override { return FName(TEXT("Pillage")); }

public:
    FName GetName() const { return Name; }
    const TMap<TSubclassOf<UItemBase>, int>& GetItems() const { return Items; }

private:
    void DestroyActorIfEmpty();

private:

    TWeakObjectPtr<UInventoryComponent> InventoryComponent;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> CreateUserWidgetClass;

    UPROPERTY(EditAnywhere, Category = "Default")
    FName Name;

    UPROPERTY(EditAnywhere, Category = "Default")
    TMap<TSubclassOf<UItemBase>, int> Items;

    UPROPERTY(EditAnywhere)
    UBoxComponent* Box;

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* ParticleSystem;
};