// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameCore/CustomStructs.h"
#include "DissolveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDissolveFinishedSignature, UPrimitiveComponent*, InComponent, bool, bInReversed);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UDissolveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDissolveComponent();

protected:

public:
    UFUNCTION(BlueprintCallable)
    void StartDissolve(UPrimitiveComponent* InComponent, bool bInReversed);
    
    UFUNCTION(BlueprintCallable)
    void StopDissolve(UPrimitiveComponent* InComponent);

private:

    void DissolveComponents();
    void RestoreComponentMaterials(int InIndex);
    void RemoveComponent(int InIndex);
    int FindComponent(UPrimitiveComponent* InComponent) const;


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

    TArray<FDissolvedComponent> DissolvedComponents;

    FTimerHandle DissolveComponentsTimerHandle;
};