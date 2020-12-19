// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameCore/CustomStructs.h"
#include "DissolveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDissolveFinishedSignature, UPrimitiveComponent*, Component, bool, bReversed);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UDissolveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDissolveComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    void StartDessolve(UPrimitiveComponent* Component, bool bReversed);
    void StopDissolve(UPrimitiveComponent* Component);
    void DissolveComponents();
    int FindComponent(UPrimitiveComponent* Component);
    void RestoreComponentMaterials(int Index);
    void RemoveComponent(int Index);

public:
    UPROPERTY(BlueprintAssignable)
        FDissolveFinishedSignature OnDissolveFinished;

private:
    UPROPERTY(EditAnywhere)
        UMaterialInstance* DissolveMaterial;

    UPROPERTY(EditAnywhere)
        FName DissolveValueName;

    UPROPERTY(EditAnywhere)
        FName DissolveColorName;

    UPROPERTY(EditAnywhere)
        float DissolveInterpSpeed;

    UPROPERTY(EditAnywhere)
        FLinearColor DissolveColor;

    UPROPERTY(EditAnywhere)
        TArray<FDissolvedComponent> DissolvedComponents;

    FTimerHandle DissolveComponentsTimerHandle;
};