// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DamageAbilityEffectBase.generated.h"

UCLASS()
class ADamageAbilityEffectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageAbilityEffectBase();

protected:

public:	
    virtual void GetTraceObjects(TArray<FHitResult>& OutHits) const {}

    void Init(float InDamageRadius, float InDamage, float InImpulse);

    void DamageToShapeArea();

    bool IsEnemy(AActor* InTarget) const;

    void ApplyImpulseToCharacter(AActor* InActor, FVector Normal);


    float GetDamageRadius() const { return DamageRadius; }

    float GetDamage() const { return Damage; }

    float GetImpulse() const { return Impulse; }


protected:

    UPROPERTY(EditAnywhere)
        float DamageRadius;

    UPROPERTY(EditAnywhere)
        float Damage;

    UPROPERTY(EditAnywhere)
        float Impulse;

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EDrawDebugTrace::Type> DebugType;
};
