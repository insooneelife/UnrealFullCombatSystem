// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpaledArrowActor.generated.h"

UCLASS()
class AImpaledArrowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpaledArrowActor();

public:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

    UFUNCTION()
    void OnDestroyed(AActor* DestroyedActor);

public:
    void Init(UStaticMesh* InMesh);

private:
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditAnywhere)
    float LifeTime;
};
