// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "AbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAbilityStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityEndedSignature, EAbilityEndResult, InResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaConsumedSignature, float, InAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityChangedSignature, AAbilityBase* const, InNewAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastingChangedSignature, bool, bInIsCasting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressedChangedSignature, bool, bInIsPressed);

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
    void UpdateSpellIndicatorLocation(FVector InNewLocation);

    UFUNCTION(BlueprintCallable)
    bool StartAbility();

    UFUNCTION(BlueprintCallable)
    void EndAbility(EAbilityEndResult InResult);

    UFUNCTION(BlueprintCallable)
    bool IsUsingAbility() const;

    UFUNCTION(BlueprintCallable)
    bool CanAbilityBeCancelled() const;

    UFUNCTION(BlueprintCallable)
    bool IsCasting() const { return bIsCasting; }

    UFUNCTION(BlueprintCallable)
    bool IsPressed() const { return bIsPressed; }

protected:
    UFUNCTION()
    void OnActiveItemChanged(
        const FStoredItem& InOldItem, const FStoredItem& InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex);

    UFUNCTION()
    void OnMainHandTypeChanged(EItemType InType);

public:
    void UpdateAbility(TSubclassOf<AAbilityBase> InAbilityClass);
    void SpawnAbility(TSubclassOf<AAbilityBase> InAbilityClass);
    void AbilityPressed();
    void AbilityReleased();
    void ConsumeMana(float InAmount);
    void ShowSpellIndicator(FVector InLocation, float InRadius, UMaterialInterface* const InMaterial);
    void HideSpellIndicator();
    float PlayAbilityMontage(UAnimMontage* const InMontage, float InPlayRate, FName InSection);
    bool IsAbilityUsingCrosshair() const;
    FTransform GetEffectTransform() const;
    float GetManaCost() const;
    bool IsCurrentAbilityValid() const;
    UTexture2D* GetAbilityCrosshair() const;
    bool ShouldRotateOnPressed() const;
    bool IsPlayingAbilityMontage() const;
    TWeakObjectPtr<ACharacter> GetCharacter() const { return Character; }
    void SetUpdateEquipmentAbility(bool bInValue) { bUpdateEquipmentAbility = bInValue; }

private:

    void UpdateAbilityFromEquipment();
    void AbilityChanged();
    void StopAbilityMontage();
    void SetIsPressed(bool bInValue);
    void SetIsCasting(bool bInValue);
    void HideIndicatorIfNotPressed();
    void CallAbilityEnded(EAbilityEndResult InResult);

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

    TWeakObjectPtr<UEquipmentComponent> EquipmentComponent;

    TWeakObjectPtr<ACharacter> Character;

    TWeakObjectPtr<AAbilityBase> CurrentAbility;

    TWeakObjectPtr<ASpellIndicatorActor> SpellIndicator;

    TWeakObjectPtr<UAnimMontage> RecentlyPlayedMontage;

    UPROPERTY(EditAnywhere)
    bool bUpdateEquipmentAbility;

    bool bIsPressed;

    bool bIsCasting;
};
