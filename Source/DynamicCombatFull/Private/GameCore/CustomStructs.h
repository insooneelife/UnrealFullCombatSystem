// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DataTypes.h"
#include "Misc/Guid.h"
#include "Animation/AnimMontage.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "CustomStructs.generated.h"

class ADisplayedItem;
class UItemBase;

/**
 *
 */
UCLASS()
class UCustomStructs : public UObject
{
    GENERATED_BODY()

};


USTRUCT(BlueprintType)
struct FCollCompHitActors
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TWeakObjectPtr<UPrimitiveComponent> Component;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<AActor*> HitActors;
};


USTRUCT(BlueprintType)
struct FCollisionComponent
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TWeakObjectPtr<UPrimitiveComponent> Component;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FName> Sockets;
};


USTRUCT(BlueprintType)
struct FDisplayedItems
{
    GENERATED_BODY()

public:
    FDisplayedItems() {}

    FDisplayedItems(const TArray<TWeakObjectPtr<ADisplayedItem>>& DisplayedItems) : DisplayedItems(DisplayedItems) {}

    UPROPERTY(EditAnywhere)
        TArray<TWeakObjectPtr<ADisplayedItem>> DisplayedItems;
};


USTRUCT(BlueprintType)
struct FDissolvedComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TWeakObjectPtr<UPrimitiveComponent> Component;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Value;

    UPROPERTY(EditAnywhere)
        TArray<TWeakObjectPtr<UMaterialInterface>> Materials;

    UPROPERTY(EditAnywhere)
        TArray<TWeakObjectPtr<UMaterialInstanceDynamic>> DissolveMaterials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bReverse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsRunning;
};

USTRUCT(BlueprintType)
struct FEffect
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EEffectType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TWeakObjectPtr<AActor> Applier;
};

USTRUCT(BlueprintType)
struct FStoredItem
{
    GENERATED_BODY()

public:
    FStoredItem() : Amount(0) { }
    FStoredItem(TSubclassOf<UItemBase> ItemClass) : ItemClass(ItemClass), Amount(0) {}
    FStoredItem(const FGuid& InId, TSubclassOf<UItemBase> InItemClass, int InAmount)
        :Id(InId), ItemClass(InItemClass), Amount(InAmount) {}

    FORCEINLINE FStoredItem& operator=(const FStoredItem& Other)
    {
        this->Id = Other.Id;
        this->ItemClass = Other.ItemClass;
        this->Amount = Other.Amount;

        return *this;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FGuid Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TSubclassOf<UItemBase> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int Amount;
};


USTRUCT(BlueprintType)
struct FEquipmentSlot
{
    GENERATED_BODY()

public:

    FEquipmentSlot() : ActiveItemIndex(0), bIsHidden(false){}
    FEquipmentSlot(const TArray<FStoredItem>& InItems, int InActiveItemIndex, bool bInIsHidden)
        :Items(InItems), ActiveItemIndex(InActiveItemIndex), bIsHidden(bInIsHidden) {}

    FORCEINLINE FEquipmentSlot& operator=(const FEquipmentSlot& Other)
    {
        this->Items.Reset(Other.Items.Num());
        this->Items.Append(Other.Items);
        this->ActiveItemIndex = Other.ActiveItemIndex;
        this->bIsHidden = Other.bIsHidden;

        return *this;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FStoredItem> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        int ActiveItemIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsHidden;
};


USTRUCT(BlueprintType)
struct FEquipmentSlots
{
    GENERATED_BODY()

public:
    FEquipmentSlots() : Type(EItemType::None) {}

    FEquipmentSlots(EItemType InType, const TArray<FEquipmentSlot>& InSlots) : Type(InType), Slots(InSlots) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FEquipmentSlot> Slots;
};


USTRUCT(BlueprintType)
struct FHitData

{
    GENERATED_BODY()

public:
    FHitData() 
        :
        Damage(0),
        HitFromDirection(FVector::ZeroVector),
        bCanBeParried(false),
        bCanBeBlocked(true),
        bCanReceiveImpact(true)
    {}

    FHitData(
        float InDamage,
        AActor* InDamageCauser,
        FVector InHitFromDirection,
        bool bInCanBeParried, 
        bool bInCanBeBlocked, 
        bool bInCanReceiveImpact) 
        : 
        Damage(InDamage),
        DamageCauser(InDamageCauser),
        HitFromDirection(InHitFromDirection),
        bCanBeParried(bInCanBeParried),
        bCanBeBlocked(bInCanBeBlocked),
        bCanReceiveImpact(bInCanReceiveImpact)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TWeakObjectPtr<AActor> DamageCauser;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FVector HitFromDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bCanBeParried;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bCanBeBlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bCanReceiveImpact;
};


USTRUCT(BlueprintType)
struct FItem
{
    GENERATED_BODY()

public:
    FItem() 
        :
        Name(NAME_None), Description(), Type(EItemType::None), bIsStackable(false), bIsDroppable(true), bIsConsumable(false), Image(nullptr)
    {}
    FItem(const FName& InName, const FText& InDescription, EItemType InType, bool bInIsStackable, bool bInIsDroppable, bool bInIsConsumable, UTexture2D* InImage)
        :
        Name(InName), Description(InDescription), Type(InType), bIsStackable(bInIsStackable), bIsDroppable(bInIsDroppable), bIsConsumable(bInIsConsumable), Image(InImage)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsStackable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsDroppable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool bIsConsumable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UTexture2D* Image;
};


USTRUCT(BlueprintType)
struct FModifier
{
    GENERATED_BODY()

public:
    FModifier() : Type(EStat::None), Value(0.0f) {}
    FModifier(EStat InType, float InValue) : Type(InType), Value(InValue){}


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EStat Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Value;
};

USTRUCT(BlueprintType)
struct FMontageActionRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EMontageAction Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<UAnimMontage*> Montages;
};


USTRUCT(BlueprintType)
struct FStat
{
    GENERATED_BODY()

public:
    FStat() : Type(EStat::None), BaseValue(0.0f), ModifiersValue(0.0f) {}

    FStat(EStat InType, float InBaseValue, float InModifiersValue)
        :Type(InType), BaseValue(InBaseValue), ModifiersValue(InModifiersValue)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EStat Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float BaseValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float ModifiersValue;
};