// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "StatsManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModifierAddedSignature, EStat, InType, float, InValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModifierRemovedSignature, EStat, InType, float, InValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBaseValueChangedSignature, EStat, InType, float, InValue);

class UEquipmentComponent;
class UItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UStatsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsManagerComponent();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    void Init();
    void TakeDamage(float InDamage, bool bInIgnoreStamina);
    void AddModifier(EStat InType, float InValue);
    void RemoveModifier(EStat InType, float InValue);
    float GetStatValue(EStat InType, bool bInIncludeModifiers) const;
    float GetDamage() const;
    int GetRecentlyReceivedHitsCount() const { return RecentlyReceivedHitsCount; }
    float GetRecentlyReceivedDamage() const { return RecentlyReceivedDamage; }
    int GetRecentlyReceivedSuccessfulHitsCount() const { return RecentlyReceivedSuccessfulHitsCount; }
    float GetRecentlyReceivedSuccessfulDamage() const { return RecentlyReceivedSuccessfulDamage; }

protected:
    UFUNCTION()
    void OnActiveItemChanged(
        FStoredItem InOldItem, FStoredItem InNewItem, EItemType InSlotType, int InSlotIndex, int InActiveIndex);

    UFUNCTION()
    void OnSlotHiddenChanged(EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden);

    UFUNCTION()
    void OnMainHandTypeSwitched(EItemType InSlotType);

private:	

    void ExcludeItemModifiers(TSubclassOf<UItemBase> InItem);
    void IncludeItemModifiers(TSubclassOf<UItemBase> InItem);
    int GetStatIndex(EStat InType) const;
    void ChangeStatBaseValue(EStat InType, float InNewValue);
    void ResetRecentlyReceivedDamage();
    void UpdateBlockBaseValue();


public:
    UPROPERTY(BlueprintAssignable)
    FModifierAddedSignature OnModifierAdded;

    UPROPERTY(BlueprintAssignable)
    FModifierRemovedSignature OnModifierRemoved;

    UPROPERTY(BlueprintAssignable)
    FBaseValueChangedSignature OnBaseValueChanged;

private:

    TWeakObjectPtr<UEquipmentComponent> EquipmentComponent;

    UPROPERTY(EditAnywhere)
    TArray<FStat> Stats;

    float InitialBlockValue;
    float RecentlyReceivedDamage;
    int RecentlyReceivedHitsCount;
    float RecentlyReceivedSuccessfulDamage;
    int RecentlyReceivedSuccessfulHitsCount;

    FTimerHandle ResetRecentlyReceivedDamageTimerHandle;
};
