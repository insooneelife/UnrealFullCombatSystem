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

    FDisplayedItems(const TArray<ADisplayedItem*>& DisplayedItems) : DisplayedItems(DisplayedItems) {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<ADisplayedItem*> DisplayedItems;
};


USTRUCT(BlueprintType)
struct FDissolvedComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UPrimitiveComponent* Component;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<UMaterialInterface*> Materials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<UMaterialInstanceDynamic*> DissolveMaterials;

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
        AActor* Applier;
};

USTRUCT(BlueprintType)
struct FStoredItem
{
    GENERATED_BODY()

public:
    FStoredItem() : Amount(0) { }
    FStoredItem(TSubclassOf<UItemBase> ItemClass) : ItemClass(ItemClass), Amount(0) {}
    FStoredItem(const FGuid& Id, TSubclassOf<UItemBase> ItemClass, int Amount)
        :Id(Id), ItemClass(ItemClass), Amount(Amount) {}

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

    FEquipmentSlots(EItemType Type, const TArray<FEquipmentSlot>& Slots) : Type(Type), Slots(Slots) {}

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        AActor* DamageCauser;

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
        Type(EItemType::None), bIsStackable(false), bIsDroppable(true), bIsConsumable(false)
    {}
    FItem(const FName& Name, const FText& Description, EItemType Type, bool bIsStackable, bool bIsDroppable, bool bIsConsumable, UTexture2D* Image)
        :
        Name(Name), Description(Description), Type(Type), bIsStackable(bIsStackable), bIsDroppable(bIsDroppable), bIsConsumable(bIsConsumable), Image(Image)
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
    FModifier(EStat Type, float Value) : Type(Type), Value(Value){}


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

    FStat(EStat Type, float BaseValue, float ModifiersValue)
        :Type(Type), BaseValue(BaseValue), ModifiersValue(ModifiersValue)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EStat Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float BaseValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float ModifiersValue;
};