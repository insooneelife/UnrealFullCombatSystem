// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

class AIPerceptionComponent;
class AAICharacter;
class APawn;

/**
 * 
 */
UCLASS()
class ABaseAIController : public AAIController
{
	GENERATED_BODY()
	

protected:
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnInCombatChanged(bool bInIsInCombat);

public:        
    void UpdateTarget();

    UFUNCTION(BlueprintCallable)
    void SetTarget(AActor* NewTarget);

    AActor* GetTarget() const { return Target; }

    void SetIsInCombat(bool bValue);

    void UpdateSenseTarget();

    bool IsEnemy(const FAIStimulus& InAIStimulus, AActor* InActor) const;
private:

    FName TargetKey;
    FName AttackTypeKey;
    FName StateKey;
    FName IsInCombatKey;

    UPROPERTY()
    UAIPerceptionComponent* AIPerception;
    
    UPROPERTY()
    AActor* Target;

    UPROPERTY()
    AAICharacter* PossesedAICharacter;

    bool bIsInCombat;
};
