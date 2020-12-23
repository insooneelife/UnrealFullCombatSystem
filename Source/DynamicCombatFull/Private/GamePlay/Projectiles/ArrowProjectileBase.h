// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "ArrowProjectileBase.generated.h"

class UParticleSystemComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UCollisionHandlerComponent;
class UPrimitiveComponent;
class UArrowItem;


UCLASS()
class AArrowProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void BeginPlayDelayed();

    UFUNCTION()
    void OnHit(const FHitResult& Hit);

public:	
    void Init(float InDamage, float InInitialSpeed);
    void ApplyHitImpulse(UPrimitiveComponent* InComponent, FVector InHitNormal, FName InBoneName);
    void UpdateArrowMesh();
    void OnArrowHit(const FHitResult& InHit);
    bool IsEnemy(AActor* InTarget) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    void SpawnImpaledArrow(USceneComponent* InComponent, FName InSocketName, AActor* InActor, FVector InLocation);

private:
    UPROPERTY(EditAnywhere)
    float Damage;

    float HeadShotDamageMultiplier;

    UPROPERTY(EditAnywhere)
    float InitialSpeed;

    float LifeTime;
    float ImpulsePower;

    UPROPERTY()
    UCollisionHandlerComponent* CollisionHandler;

    UPROPERTY()
    UParticleSystemComponent* ParticleSystem;

public:
    UPROPERTY(BlueprintReadOnly)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(BlueprintReadOnly)
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UArrowItem> SpawnImpaledArrowClass;
};
