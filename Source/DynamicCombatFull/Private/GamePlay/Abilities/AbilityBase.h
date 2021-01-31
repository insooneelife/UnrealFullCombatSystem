// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCore/DataTypes.h"
#include "AbilityBase.generated.h"

class UAbilityComponent;
class ACharacter;
class UTexture2D;
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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void NativeInit(UAbilityComponent* InAbilityComponent);

public:
    virtual void Released() { K2_Released(); }
    virtual void Pressed() { K2_Pressed(); }
    virtual void Effect() { K2_Effect(); }
    virtual void Ended(EAbilityEndResult Result) { K2_Ended(Result); }

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint", meta = (DisplayName = "Released", ScriptName = "Released"))
    void K2_Released();

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint", meta = (DisplayName = "Pressed", ScriptName = "Pressed"))
    void K2_Pressed();

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint", meta = (DisplayName = "Effect", ScriptName = "Effect"))
    void K2_Effect();

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint", meta = (DisplayName = "Ended", ScriptName = "Ended"))
    void K2_Ended(EAbilityEndResult Result);

    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint", meta = (DisplayName = "Init", ScriptName = "Init"))
    void K2_Init();

    void Init(UAbilityComponent* InAbilityComponent) { NativeInit(InAbilityComponent); K2_Init(); }

    UFUNCTION(BlueprintCallable)
    void ConsumeManaAmount(float Amount);

    UFUNCTION(BlueprintCallable)
    void ConsumeMana();

    UFUNCTION(BlueprintCallable)
    void ShowIndicator(FVector Location);

    UFUNCTION(BlueprintCallable)
    void UpdateIndicator(FVector NewLocation);

    UFUNCTION(BlueprintCallable)
    void HideIndicator();

    UFUNCTION(BlueprintCallable)
    float PlayAbilityMontage(UAnimMontage* Montage, FName StartSectionName, bool bUseCastingSpeed, float PlayRate);
    
    UFUNCTION(BlueprintCallable)
    float GetDamage() const;

    UFUNCTION(BlueprintCallable)
    UAnimMontage* GetAbilityMontage(int Index) const;

    UFUNCTION(BlueprintCallable)
    void ApplyImpulseToCharacter(AActor* Actor, FVector HitNormal, float Impulse);

    UFUNCTION(BlueprintCallable)
    bool IsInHeightRange(FVector Location, float MaxHeightDeviation) const;

    UFUNCTION(BlueprintCallable)
    float GetCastingSpeed() const;

    UFUNCTION(BlueprintCallable)
    bool FindGroundLocation(
        FVector StartLocation, float GroundOffset, FVector& OutGroundLocation, FVector& OutNormal) const;

public:
    UFUNCTION(BlueprintCallable)
    bool IsUsingCrosshair() const { return bIsUsingCrosshair; }

    UFUNCTION(BlueprintCallable)
    float GetManaCost() const { return ManaCost; }

    UFUNCTION(BlueprintCallable)
    bool CanBeCancelled() const { return bCanBeCancelled; }

    UFUNCTION(BlueprintCallable)
    UTexture2D* GetCrosshairTexture() const { return CrosshairTexture; }

    UFUNCTION(BlueprintCallable)
    bool IsRotateOnPressed() const { return bRotateOnPressed; }

    UFUNCTION(BlueprintCallable)
    UAbilityComponent* GetAbilityComponent() const { return AbilityComponent.Get(); }

    UFUNCTION(BlueprintCallable)
    FTransform GetEffectTransform() const { return EffectTransform; }

    UFUNCTION(BlueprintCallable)
    FVector GetEffectLocation() const { return EffectTransform.GetLocation(); }

    UFUNCTION(BlueprintCallable)
    void SetEffectTransform(FTransform InEffectTransform) { EffectTransform = InEffectTransform; }

    UFUNCTION(BlueprintCallable)
    FName GetBeamSocket() const { return BeamSocket; }

    UFUNCTION(BlueprintCallable)
    ACharacter* GetCharacter() const { return Character.Get(); }

    UFUNCTION(BlueprintCallable)
    FName GetLeftHandSocket() const { return LeftHandSocket; }

    UFUNCTION(BlueprintCallable)
    FName GetRightHandSocket() const { return RightHandSocket; }

    UFUNCTION(BlueprintCallable)
    TArray<UAnimMontage*> GetAbilityMontages() const { return AbilityMontages; }

    UFUNCTION(BlueprintCallable)
    float GetImpulsePower() const { return ImpulsePower; }

protected:

    TWeakObjectPtr<UAbilityComponent> AbilityComponent;
    
    FTransform EffectTransform;

    TWeakObjectPtr<ACharacter> Character;

    UPROPERTY(EditAnywhere, Category = "Ability\|Crosshair")
    UTexture2D* CrosshairTexture;

    UPROPERTY(EditAnywhere, Category = "Ability\|Indicator")
    UMaterialInterface* IndicatorMaterial;

    UPROPERTY(EditAnywhere, Category = "Ability")
    TArray<UAnimMontage*> AbilityMontages;

    UPROPERTY(EditAnywhere, Category = "Ability\|Crosshair")
    bool bIsUsingCrosshair;

    UPROPERTY(EditAnywhere, Category = "Ability\|Damage")
    float Damage;

    UPROPERTY(EditAnywhere, Category = "Ability\|Damage")
    bool bIncludeOwnerDamage;

    UPROPERTY(EditAnywhere, Category = "Ability\|Damage")
    float OwnerDamageScalar;

    UPROPERTY(EditAnywhere, Category = "Ability\|Damage")
    float ImpulsePower;


    UPROPERTY(EditAnywhere, Category = "Ability\|Sockets")
    FName LeftHandSocket;

    UPROPERTY(EditAnywhere, Category = "Ability\|Sockets")
    FName RightHandSocket;

    UPROPERTY(EditAnywhere, Category = "Ability\|Sockets")
    FName BeamSocket;


    UPROPERTY(EditAnywhere, Category = "Ability\|Indicator")
    float IndicatorRadius;

    UPROPERTY(EditAnywhere, Category = "Ability")
    bool bCanBeCancelled;

    UPROPERTY(EditAnywhere, Category = "Ability")
    bool bRotateOnPressed;

    UPROPERTY(EditAnywhere, Category = "Ability")
    float ManaCost;

};
