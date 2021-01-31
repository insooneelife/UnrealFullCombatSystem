// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionHandlerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitSignature, const FHitResult&, InHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollisionActivatedSignature, ECollisionPart, InCollisionPart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollisionDeactivatedSignature);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UCollisionHandlerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UCollisionHandlerComponent();

public:
    // Called every frame
    virtual void TickComponent(
        float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable)
    void DeactivateCollision();

    UFUNCTION(BlueprintCallable)
    void ActivateCollision(ECollisionPart InCollisionPart);

public:
    void SetCollisionMeshes(const TArray<FCollisionComponent>& InCollComps);
    void SetCollisionMesh(UPrimitiveComponent* const InWeaponMesh, const TArray<FName>& InSockets);
    void AddIgnoredClass(TSubclassOf<AActor> IgnoredClass);
    void AddIgnoredCollisionProfileNames(FName InProfileName);
    void AddObjectTypesToCollideWith(TEnumAsByte<EObjectTypeQuery> InObjectType);
    void SetTraceRadius(float InTraceRadius) { TraceRadius = InTraceRadius; }

private:

    void UpdateLastSocketPositions();
    void PerformTrace();
    bool IsIgnoredClass(TSubclassOf<AActor> InTestClass) const;

    bool IsCollisionActive() const;

    FName GetUniqueSocketName(UPrimitiveComponent* const InComponent, FName InSocketName) const;

    int GetHitActorsIndex(UPrimitiveComponent* const InComponent) const;

    // #fix architecture
    TArray<TWeakObjectPtr<AActor>> GetHitActorsOrAddOwner(UPrimitiveComponent* const InComponent);

    void AddHitActor(UPrimitiveComponent* const InComponent, AActor* const InHitActor);

public:
    UPROPERTY(BlueprintAssignable)
    FHitSignature OnHit;

    UPROPERTY(BlueprintAssignable)
    FCollisionActivatedSignature OnCollisionActivated;

    UPROPERTY(BlueprintAssignable)
    FCollisionDeactivatedSignature OnCollisionDeactivated;

private:

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToCollideWith;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EDrawDebugTrace::Type> VisualizeTrace;

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<AActor>> IgnoredClasses;

    UPROPERTY(EditAnywhere)
    TArray<FName> IgnoredCollisionProfileNames;

    UPROPERTY(EditAnywhere)
    float TraceRadius;

    UPROPERTY(EditAnywhere)
    TArray<FCollisionComponent> CollisionComponents;

    UPROPERTY(EditAnywhere)
    TArray<FCollCompHitActors> HitActors;

    bool bCanPerformTrace;
    bool bCollisionActive;
    TMap<FName, FVector> LastSocketLocations;
};