// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "AbilityBase.generated.h"

class UAbilityComponent;
class ACharacter;
class UTextrue2D;
class UMaterialInterface;
class UAnimMontage;

UCLASS()
class AAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    virtual void OnConstruction(const FTransform& Transform) override {}
    virtual void Released() {}
    virtual void Pressed() {}
    virtual void Effect() {}
    virtual void Ended(EAbilityEndResult Result) {}

    void ConsumeManaAmount(float Amount);
    void ConsumeMana();
    void ShowIndicator(FVector Location);
    void UpdateIndicator(FVector NewLocation);
    void HideIndicator();

    float PlayAbilityMontage(UAnimMontage* Montage, FName StartSectionName, bool bUseCastingSpeed, float PlayRate);
    FVector GetEffectLocation() const;
    float GetDamage() const;
    UAnimMontage* GetAbilityMontage(int Index) const;
    void ApplyImpulseToCharacter(AActor* Actor, FVector HitNormal, float Impulse);
    bool IsInHeightRange(FVector Location, float MaxHeightDeviation) const;

    float GetCastingSpeed() const;
    bool FindGroundLocation(
        FVector StartLocation, float GroundOffset, FVector& OutGroundLocation, FVector& OutNormal) const;

public:
    void SetAbilityComponent(UAbilityComponent* InAbilityComponent) { AbilityComponent = InAbilityComponent; }

    bool IsUsingCrosshair() const { return bIsUsingCrosshair; }

    FTransform GetEffectTransform() const { return EffectTransform; }

    float GetManaCost() const { return ManaCost; }

    bool CanBeCancelled() const { return bCanBeCancelled; }

    UTexture2D* GetCrosshairTexture() const { return CrosshairTexture; }

    bool IsRotateOnPressed() const { return bRotateOnPressed; }

protected:
    UPROPERTY()
    UAbilityComponent* AbilityComponent;
    
    FTransform EffectTransform;

    UPROPERTY()
    ACharacter* Character;

    bool bIsUsingCrosshair;

    UPROPERTY()
    UTexture2D* CrosshairTexture;

    float Damage;

    bool bIncludeOwnerDamage;

    float OwnerDamageScalar;

    float ImpulsePower;

    FName LeftHandSocket;

    FName RightHandSocket;

    FName BeamSocket;

    float IndicatorRadius;

    UPROPERTY()
    UMaterialInterface* IndicatorMaterial;

    UPROPERTY()
    TArray<UAnimMontage*> AbilityMontages;

    bool bCanBeCancelled;

    bool bRotateOnPressed;

    float ManaCost;

};
