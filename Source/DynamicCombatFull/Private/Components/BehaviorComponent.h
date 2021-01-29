// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBehaviorComponent();

public:
    UFUNCTION(BlueprintCallable)
    bool IsEnemy(const AActor* const InActor) const;

    void AddEnemy(TSubclassOf<AActor> Enemy);

private:
    bool CheckClass(const AActor* const InActor, const TArray<TSubclassOf<AActor>>& InEnemies) const;

private:
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<AActor>> Enemies;
		
};
