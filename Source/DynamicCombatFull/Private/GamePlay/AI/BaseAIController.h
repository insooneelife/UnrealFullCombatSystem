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
	
public:
    ABaseAIController();

protected:
    virtual void OnPossess(APawn* InPawn) override;

public:        
    UFUNCTION()
    void UpdateTarget();

    UFUNCTION(BlueprintCallable)
    void SetTarget(AActor* InNewTarget);

    UFUNCTION(BlueprintCallable)
    AActor* GetTarget() const { return Target; }
public:

    void SetIsInCombat(bool bInValue);
    void UpdateSenseTarget();
    bool IsEnemy(const FAIStimulus& InAIStimulus, AActor* InActor) const;

    UAIPerceptionComponent* GetAIPerception() const { return AIPerception; }

protected:
    UFUNCTION()
    void OnInCombatChanged(bool bInIsInCombat);


private:
    UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
    FName TargetKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
    FName AttackTypeKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
    FName StateKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
    FName IsInCombatKey;

    UPROPERTY(EditAnywhere, Category = "Components")
    UAIPerceptionComponent* AIPerception;
    
    UPROPERTY()
    AActor* Target;

    UPROPERTY()
    AAICharacter* PossesedAICharacter;

    bool bIsInCombat;
};
