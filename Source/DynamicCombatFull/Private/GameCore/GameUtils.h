// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"

class UExtendedStatComponent;
/**
 * 
 */
class GameUtils
{
public:
    static const FLinearColor Red;
    static const FLinearColor Black;
    static const FLinearColor Gray;
    static const FLinearColor Brown;

    static const FItem& GetDefaultItemFromStoredItem(const FStoredItem& StoredItem);
    static const FItem& GetDefaultItemFromItemClass(TSubclassOf<UItemBase> ItemClass);

    // logs
    static void PrintStoredItem(const FStoredItem& InStoredItem);
    static void PrintHitResult(const FHitResult& InHitResult);
    static void PrintHitData(const FHitData& InHitData);

    // editor logs
    static void PrintEffects(
        UObject* WorldContextObject,const FString& InDisplayName, const TArray<FEffect>& InEffects);

    // for easy debug
    static bool IsValid(const UObjectBase* const InObject);
    static FString GetDebugName(const UObject* const InObject);

    template<typename ClassType>
    static TSubclassOf<ClassType> LoadAssetClass(FString AssetPath)
    {
        ConstructorHelpers::FClassFinder<ClassType> ClassFinder(*AssetPath);
        if (ClassFinder.Class == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("failed to load : %s"), *AssetPath);
            return nullptr;
        }
        else
        {
            return ClassFinder.Class;
        }
    }

    template<typename ClassType>
    static ClassType* LoadAssetObject(FString AssetPath)
    {
        ConstructorHelpers::FObjectFinder<ClassType> ObjectFinder(*AssetPath);

        if (ObjectFinder.Object == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("failed to load : %s"), *AssetPath);
            return nullptr;
        }
        else
        {
            return ObjectFinder.Object;
        }
    }

    template<typename TEnum>
    static FORCEINLINE FString GetEnumValueAsString(const FString& InEnumName, TEnum InEnumValue)
    {
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *InEnumName, true);
        if (!EnumPtr) return FString("Invalid");
        return EnumPtr->GetNameByValue((int64)InEnumValue).ToString();
    }

    template<typename TEnum>
    static FORCEINLINE FString GetEnumDisplayNameAsString(const FString& InEnumName, TEnum InEnumValue)
    {
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *InEnumName, true);
        if (!EnumPtr) return FString("Invalid");
        return EnumPtr->GetDisplayNameTextByValue((int64)InEnumValue).ToString();
    }

    template <typename EnumType>
    static FORCEINLINE EnumType GetEnumValueFromString(const FString& InEnumName, const FString& InString)
    {
        UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *InEnumName, true);
        if (!Enum) {
            return EnumType(0);
        }
        return (EnumType)Enum->FindEnumIndex(FName(*InString));
    }
};
