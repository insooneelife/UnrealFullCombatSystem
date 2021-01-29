// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBehaviorComponent::UBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

bool UBehaviorComponent::IsEnemy(const AActor* const InActor) const
{
    return CheckClass(InActor, Enemies);
}

void UBehaviorComponent::AddEnemy(TSubclassOf<AActor> Enemy)
{
    Enemies.Add(Enemy);
}

bool UBehaviorComponent::CheckClass(const AActor* const InActor, const TArray<TSubclassOf<AActor>>& InEnemies) const
{
    for (TSubclassOf<AActor> ActorClass : InEnemies)
    {
        if (UKismetMathLibrary::ClassIsChildOf(InActor->GetClass(), ActorClass))
        {
            return true;
        }
    }
    return false;
}