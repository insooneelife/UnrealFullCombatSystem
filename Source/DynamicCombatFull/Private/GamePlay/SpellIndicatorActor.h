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
	
public:	
    void Init(float InRadius, UMaterialInterface* InMaterial);

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
