// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionHandlerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitSignature, const FHitResult&, Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollisionActivatedSignature, ECollisionPart, CollisionPart);
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
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable)
    void DeactivateCollision();

    UFUNCTION(BlueprintCallable)
    void ActivateCollision(ECollisionPart CollisionPart);

public:
    void SetCollisionMeshes(const TArray<FCollisionComponent>& CollComps);
    void SetCollisionMesh(UPrimitiveComponent* WeaponMesh, const TArray<FName>& Sockets);

private:

    void UpdateLastSocketPositions();
    void PerformTrace();
    bool IsIgnoredClass(TSubclassOf<AActor> TestClass) const;

    bool IsCollisionActive() const;

    FName GetUniqueSocketName(UPrimitiveComponent* Component, FName SocketName) const;

    int GetHitActorsIndex(UPrimitiveComponent* Component) const;

    TArray<AActor*> GetHitActors(UPrimitiveComponent* Component);

    void AddHitActor(UPrimitiveComponent* Component, AActor* HitActor);

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