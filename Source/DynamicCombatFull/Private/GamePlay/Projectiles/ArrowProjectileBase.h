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
class AImpaledArrowActor;


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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void BeginPlayDelayed();

    UFUNCTION()
    void OnHit(const FHitResult& InHit);

    virtual void OnArrowHit(const FHitResult& InHit);

public:	
    void Init(float InDamage, float InInitialSpeed);

    UFUNCTION(BlueprintCallable)
    void ApplyHitImpulse(UPrimitiveComponent* InComponent, FVector InHitNormal, FName InBoneName);
    void UpdateArrowMesh();

    UFUNCTION(BlueprintCallable)
    bool IsEnemy(AActor* InTarget) const;

    //UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
    void SpawnImpaledArrow(USceneComponent* InComponent, FName InSocketName, AActor* InActor, FVector InLocation);

    UFUNCTION(BlueprintCallable)
    float GetDamage() const { return Damage; }

    UFUNCTION(BlueprintCallable)
        float GetHeadShotDamageMultiplier() const { return HeadShotDamageMultiplier; }

    UFUNCTION(BlueprintCallable)
        float GetInitialSpeed() const { return InitialSpeed; }

    UFUNCTION(BlueprintCallable)
        float GetLifeTime() const { return LifeTime; }

    UFUNCTION(BlueprintCallable)
        float GetImpulsePower() const { return ImpulsePower; }

protected:
    UPROPERTY(EditAnywhere, Category = "Projectile")
    float Damage;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float HeadShotDamageMultiplier;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float InitialSpeed;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float LifeTime;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float ImpulsePower;

    UPROPERTY(EditAnywhere)
    UCollisionHandlerComponent* CollisionHandler;

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* ParticleSystem;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AImpaledArrowActor> ImpaledArrowClass;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UProjectileMovementComponent* ProjectileMovement;
};
