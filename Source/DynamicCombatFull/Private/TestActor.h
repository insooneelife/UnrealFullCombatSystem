// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CanOpenUI.h"
#include "TestActor.generated.h"

UCLASS()
class ATestActor : public AActor, public ICanOpenUI
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    virtual void OpenedUI() override {}
    virtual void ClosedUI() override {}
public:
    int Data;
};
