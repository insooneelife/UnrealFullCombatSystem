// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "AbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityEndedSignature, EAbilityEndResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaConsumedSignature, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityChangedSignature, AAbilityBase*, NewAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastingChangedSignature, bool, bIsCasting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressedChangedSignature, bool, bIsPressed);

class AAbilityBase;
class UEquipmentComponent;
class UCharacter;
class UAnimMontage;
class ASpellIndicatorActor;
class UMaterialInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    UFUNCTION(BlueprintCallable)
    void AbilityEffect();

    UFUNCTION(BlueprintCallable)
    void UpdateSpellIndicatorLocation(FVector NewLocation);

    UFUNCTION(BlueprintCallable)
    bool StartAbility();

    UFUNCTION(BlueprintCallable)
    void EndAbility(EAbilityEndResult Result);

    UFUNCTION(BlueprintCallable)
    bool IsCasting() const { return bIsCasting; }

    UFUNCTION(BlueprintCallable)
    bool IsPressed() const { return bIsPressed; }

    UFUNCTION(BlueprintCallable)
    bool IsUsingAbility() const;

    UFUNCTION(BlueprintCallable)
    bool CanAbilityBeCancelled() const;

protected:
    UFUNCTION()
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex);

    UFUNCTION()
    void OnMainHandTypeChanged(EItemType Type);

public:
    void AbilityPressed();
    void AbilityReleased();
    void ConsumeMana(float Amount);
    void ShowSpellIndicator(FVector InLocation, float InRadius, UMaterialInterface* InMaterial);
    void HideSpellIndicator();
    float PlayAbilityMontage(UAnimMontage* Montage, float PlayRate, FName Section);
    bool IsAbilityUsingCrosshair() const;
    FTransform GetEffectTransform() const;
    float GetManaCost() const;
    bool IsCurrentAbilityValid() const;
    UTexture2D* GetAbilityCrosshair() const;
    bool ShouldRotateOnPressed() const;
    bool IsPlayingAbilityMontage() const;
    ACharacter* GetCharacter() const { return Character; }

private:

    void UpdateAbilityFromEquipment();
    void AbilityChanged();
    void UpdateAbility(TSubclassOf<AAbilityBase> Ability);
    void StopAbilityMontage();
    void SetIsPressed(bool bValue);
    void SetIsCasting(bool bValue);
    void HideIndicatorIfNotPressed();
    void CallAbilityEnded(EAbilityEndResult Result);

public:
    UPROPERTY(BlueprintAssignable)
    FAbilityStartedSignature OnAbilityStarted;

    UPROPERTY(BlueprintAssignable)
    FAbilityEndedSignature OnAbilityEnded;

    UPROPERTY(BlueprintAssignable)
    FManaConsumedSignature OnManaConsumed;

    UPROPERTY(BlueprintAssignable)
    FAbilityChangedSignature OnAbilityChanged;

    UPROPERTY(BlueprintAssignable)
    FCastingChangedSignature OnCastingChanged;

    UPROPERTY(BlueprintAssignable)
    FPressedChangedSignature OnPressedChanged;

private:

    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<ASpellIndicatorActor> SpawnIndicatorClass;

    UPROPERTY()
    UEquipmentComponent* EquipmentComponent;

    UPROPERTY()
    ACharacter* Character;

    UPROPERTY()
    AAbilityBase* CurrentAbility;

    UPROPERTY()
    ASpellIndicatorActor* SpellIndicator;

    UPROPERTY()
    UAnimMontage* RecentlyPlayedMontage;

    UPROPERTY(EditAnywhere)
    bool bUpdateEquipmentAbility;

    bool bIsPressed;

    bool bIsCasting;
};
