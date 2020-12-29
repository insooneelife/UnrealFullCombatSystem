// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellIndicatorActor.generated.h"

class UDecalComponent;
class UMaterialInterface;

UCLASS()
class ASpellIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellIndicatorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    virtual void OnConstruction(const FTransform& Transform) override;

    void SetRadius(float NewRadius);
    void SetMaterial(UMaterialInterface* NewMaterial);

    void Show();
    void Hide();


private:
    UPROPERTY(EditAnywhere)
    USceneComponent* Scene;

    UPROPERTY(EditAnywhere)
    UDecalComponent* Decal;

    UPROPERTY(EditAnywhere)
    float Radius;

};
