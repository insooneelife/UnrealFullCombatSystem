// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "StatsManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModifierAddedSignature, EStat, Type, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModifierRemovedSignature, EStat, Type, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBaseValueChangedSignature, EStat, Type, float, Value);

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
	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    void OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType SlotType, int SlotIndex, int ActiveIndex);

    UFUNCTION()
    void OnSlotHiddenChanged(EItemType SlotType, int SlotIndex, FStoredItem ActiveItem, bool bIsHidden);

    UFUNCTION()
    void OnMainHandTypeSwitched(EItemType SlotType);

public:	

    void Init();

    void ExcludeItemModifiers(TSubclassOf<UItemBase> Item);
    void IncludeItemModifiers(TSubclassOf<UItemBase> Item);
    void AddModifier(EStat Type, float Value);
    void RemoveModifier(EStat Type, float Value);
    float GetStatValue(EStat Type, bool bIncludeModifiers) const;
    int GetStatIndex(EStat Type) const;
    float GetDamage() const;
    void ChangeStatBaseValue(EStat Type, float NewValue);

    void TakeDamage(float Damage, bool bIgnoreStamina);
    void ResetRecentlyReceivedDamage();
    void UpdateBlockBaseValue();

    float GetRecentlyReceivedDamage() const { return RecentlyReceivedDamage; }
    int GetRecentlyReceivedHitsCount() const { return RecentlyReceivedHitsCount; }
    int GetRecentlyReceivedSuccessfulHitsCount() const { return RecentlyReceivedSuccessfulHitsCount; }
    float GetRecentlyReceivedSuccessfulDamage() const { return RecentlyReceivedSuccessfulDamage; }

public:
    UPROPERTY(BlueprintAssignable)
    FModifierAddedSignature OnModifierAdded;

    UPROPERTY(BlueprintAssignable)
    FModifierRemovedSignature OnModifierRemoved;

    UPROPERTY(BlueprintAssignable)
    FBaseValueChangedSignature OnBaseValueChanged;

private:
    UPROPERTY()
    UEquipmentComponent* EquipmentComponent;

    UPROPERTY(EditAnywhere)
    TArray<FStat> Stats;

    float InitialBlockValue;
    float RecentlyReceivedDamage;
    int RecentlyReceivedHitsCount;
    float RecentlyReceivedSuccessfulDamage;
    int RecentlyReceivedSuccessfulHitsCount;

    FTimerHandle ResetRecentlyReceivedDamageTimerHandle;
};
