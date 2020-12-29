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

    UPROPERTY(EditAnywhere)
        UPrimitiveComponent* Component;

    UPROPERTY(EditAnywhere)
        TArray<AActor*> HitActors;
};


USTRUCT(BlueprintType)
struct FCollisionComponent
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
        UPrimitiveComponent* Component;

    UPROPERTY(EditAnywhere)
        TArray<FName> Sockets;
};


USTRUCT(BlueprintType)
struct FDisplayedItems
{
    GENERATED_BODY()

public:
    FDisplayedItems() {}

    FDisplayedItems(const TArray<ADisplayedItem*>& DisplayedItems) : DisplayedItems(DisplayedItems) {}

    UPROPERTY(EditAnywhere)
        TArray<ADisplayedItem*> DisplayedItems;
};


USTRUCT(BlueprintType)
struct FDissolvedComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        UPrimitiveComponent* Component;

    UPROPERTY(EditAnywhere)
        float Value;

    UPROPERTY(EditAnywhere)
        TArray<UMaterialInterface*> Materials;

    UPROPERTY(EditAnywhere)
        TArray<UMaterialInstanceDynamic*> DissolveMaterials;

    UPROPERTY(EditAnywhere)
        bool bReverse;

    UPROPERTY(EditAnywhere)
        bool bIsRunning;
};

USTRUCT(BlueprintType)
struct FEffect
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        EEffectType Type;

    UPROPERTY(EditAnywhere)
        float Duration;

    UPROPERTY(EditAnywhere)
        AActor* Applier;
};

USTRUCT(BlueprintType)
struct FStoredItem
{
    GENERATED_BODY()

public:
    FStoredItem() : Amount(0) { }
    FStoredItem(const FGuid& Id, TSubclassOf<UItemBase> ItemClass, int Amount)
        :Id(Id), ItemClass(ItemClass), Amount(Amount) {}

    FORCEINLINE FStoredItem& operator=(const FStoredItem& Other)
    {
        this->Id = Other.Id;
        this->ItemClass = Other.ItemClass;
        this->Amount = Other.Amount;

        return *this;
    }

    UPROPERTY(EditAnywhere)
        FGuid Id;

    UPROPERTY(EditAnywhere)
        TSubclassOf<UItemBase> ItemClass;

    UPROPERTY(EditAnywhere)
        int Amount;
};


USTRUCT(BlueprintType)
struct FEquipmentSlot
{
    GENERATED_BODY()

public:

    FEquipmentSlot() : ActiveItemIndex(0), bIsHidden(false){}
    FEquipmentSlot(const TArray<FStoredItem>& Items, int ActiveItemIndex, bool bIsHidden)
        :Items(Items), ActiveItemIndex(ActiveItemIndex), bIsHidden(bIsHidden) {}

    FORCEINLINE FEquipmentSlot& operator=(const FEquipmentSlot& Other)
    {
        this->Items.Reset(Other.Items.Num());
        this->Items.Append(Other.Items);
        this->ActiveItemIndex = Other.ActiveItemIndex;
        this->bIsHidden = Other.bIsHidden;

        return *this;
    }

    UPROPERTY(EditAnywhere)
        TArray<FStoredItem> Items;

    UPROPERTY(EditAnywhere)
        int ActiveItemIndex;

    UPROPERTY(EditAnywhere)
        bool bIsHidden;
};


USTRUCT(BlueprintType)
struct FEquipmentSlots
{
    GENERATED_BODY()

public:
    FEquipmentSlots() : Type(EItemType::None) {}

    FEquipmentSlots(EItemType Type, const TArray<FEquipmentSlot>& Slots) : Type(Type), Slots(Slots) {}

    UPROPERTY(EditAnywhere)
        EItemType Type;

    UPROPERTY(EditAnywhere)
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
        bCanBeParried(false),
        bCanBeBlocked(true),
        bCanReceiveImpact(true)
    {}

    FHitData(
        float Damage,
        AActor* DamageCauser,
        FVector HitFromDirection,
        bool bCanBeParried, 
        bool bCanBeBlocked, 
        bool bCanReceiveImpact) 
        : 
        Damage(Damage),
        DamageCauser(DamageCauser),
        HitFromDirection(HitFromDirection), 
        bCanBeParried(bCanBeParried), 
        bCanBeBlocked(bCanBeBlocked), 
        bCanReceiveImpact(bCanReceiveImpact) 
    {}

    UPROPERTY(EditAnywhere)
        float Damage;

    UPROPERTY(EditAnywhere)
        AActor* DamageCauser;

    UPROPERTY(EditAnywhere)
        FVector HitFromDirection;

    UPROPERTY(EditAnywhere)
        bool bCanBeParried;

    UPROPERTY(EditAnywhere)
        bool bCanBeBlocked;

    UPROPERTY(EditAnywhere)
        bool bCanReceiveImpact;
};


USTRUCT(BlueprintType)
struct FItem
{
    GENERATED_BODY()

public:
    FItem() 
        :
        Type(EItemType::None), bIsStackable(false), bIsDroppable(true), bIsConsumable(false)
    {}
    FItem(const FName& Name, const FText& Description, EItemType Type, bool bIsStackable, bool bIsDroppable, bool bIsConsumable, UTexture2D* Image)
        :
        Name(Name), Description(Description), Type(Type), bIsStackable(bIsStackable), bIsDroppable(bIsDroppable), bIsConsumable(bIsConsumable), Image(Image)
    {}

    UPROPERTY(EditAnywhere)
        FName Name;

    UPROPERTY(EditAnywhere)
        FText Description;

    UPROPERTY(EditAnywhere)
        EItemType Type;

    UPROPERTY(EditAnywhere)
        bool bIsStackable;

    UPROPERTY(EditAnywhere)
        bool bIsDroppable;

    UPROPERTY(EditAnywhere)
        bool bIsConsumable;

    UPROPERTY(EditAnywhere)
        UTexture2D* Image;
};


USTRUCT(BlueprintType)
struct FModifier
{
    GENERATED_BODY()

public:
    FModifier() : Type(EStat::None), Value(0.0f) {}
    FModifier(EStat Type, float Value) : Type(Type), Value(Value){}


    UPROPERTY(EditAnywhere)
        EStat Type;

    UPROPERTY(EditAnywhere)
        float Value;
};

USTRUCT(BlueprintType)
struct FMontageAction
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
        EMontageAction Type;

    UPROPERTY(EditAnywhere)
        TArray<UAnimMontage*> Montages;
};


USTRUCT(BlueprintType)
struct FStat
{
    GENERATED_BODY()

public:
    FStat() : Type(EStat::None), BaseValue(0.0f), ModifiersValue(0.0f) {}

    FStat(EStat Type, float BaseValue, float ModifiersValue)
        :Type(Type), BaseValue(BaseValue), ModifiersValue(ModifiersValue)
    {}

    UPROPERTY(EditAnywhere)
        EStat Type;

    UPROPERTY(EditAnywhere)
        float BaseValue;

    UPROPERTY(EditAnywhere)
        float ModifiersValue;
};