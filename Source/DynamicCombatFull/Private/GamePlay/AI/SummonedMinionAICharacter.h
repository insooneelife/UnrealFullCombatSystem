// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/AI/AICharacter.h"
#include "SummonedMinionAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASummonedMinionAICharacter : public AAICharacter
{
	GENERATED_BODY()
	
public:
    // Sets default values for this character's properties
    ASummonedMinionAICharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayResult) override;

    UFUNCTION()
    void OnOwnerStateChanged(EState InPrevState, EState InNewState);

    UFUNCTION()
    void OnCollisionActivatedOverride(ECollisionPart CollisionPart);

public:
    // ICanGetEffects
    virtual bool CanEffectBeApplied(EEffectType Type, AActor* Applier) override;


private:
    void Delayed_BeginPlay();

    void DissolveInCharacterMesh();
    void Delayed_OnCollisionActivated();
    void Delayed_Delayed_OnCollisionActivated();

private:
    UPROPERTY(EditAnywhere)
    TArray<FName> CollisionSockets;

    UPROPERTY(EditAnywhere)
    float Duration;


    FTimerHandle DeathTimerHandle;
};
