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
    UFUNCTION()
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex);

    UFUNCTION()
    void OnMainHandTypeChanged(EItemType Type);

    void UpdateAbilityFromEquipment();

    void AbilityPressed();

    void AbilityReleased();

    UFUNCTION(BlueprintCallable)
    void AbilityEffect();

    void ConsumeMana(float Amount);

    void AbilityChanged();

public:
    void ShowSpellIndicator(FVector Location, float Radius, UMaterialInterface* Material);

    void HideSpellIndicator();

    UFUNCTION(BlueprintCallable)
    void UpdateSpellIndicatorLocation(FVector NewLocation);

    void UpdateAbility(TSubclassOf<AAbilityBase> Ability);

    UFUNCTION(BlueprintCallable)
    bool StartAbility();

    float PlayAbilityMontage(UAnimMontage* Montage, float PlayRate, FName Section);

    void StopAbilityMontage();

    UFUNCTION(BlueprintCallable)
    void EndAbility(EAbilityEndResult Result);

public:
    AAbilityBase* GetCurrentAbility() const;
    bool IsAbilityUsingCrosshair() const;
    FTransform GetEffectTransform() const;

    UFUNCTION(BlueprintCallable)
    bool GetIsCasting() const;

    UFUNCTION(BlueprintCallable)
    bool GetIsPressed() const;

    UFUNCTION(BlueprintCallable)
    bool IsUsingAbility() const;
    float GetManaCost() const;

    UFUNCTION(BlueprintCallable)
    bool CanAbilityBeCancelled() const;
    bool IsAbilityValid() const;
    UTexture2D* GetAbilityCrosshair() const;
    bool ShouldRotateOnPressed() const;
    void SetIsPressed(bool bValue);
    void SetIsCasting(bool bValue);
    bool IsPlayingAbilityMontage() const;

    void HideIndicatorIfNotPressed();
    void CallAbilityEnded(EAbilityEndResult Result);

    ACharacter* GetCharacter() const { return Character; }

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

    UPROPERTY(EditAnywhere)
        TSubclassOf<ASpellIndicatorActor> SpawnIndicatorClass;

    UPROPERTY()
        UEquipmentComponent* EquipmentComponent;

    UPROPERTY()
        AAbilityBase* CurrentAbility;

    bool bIsPressed;

    bool bIsCasting;

    UPROPERTY()
    ACharacter* Character;

    UPROPERTY()
    ASpellIndicatorActor* SpellIndicator;

    UPROPERTY()
    UAnimMontage* RecentlyPlayedMontage;

    UPROPERTY(EditAnywhere)
    bool bUpdateEquipmentAbility;
};
