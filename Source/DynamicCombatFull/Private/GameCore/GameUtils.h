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
    static void PrintStoredItem(const FStoredItem& StoredItem);

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
    static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
    {
        const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
        if (!enumPtr) return FString("Invalid");
        return enumPtr->GetNameByValue((int64)Value).ToString();
    }

    template<typename TEnum>
    static FORCEINLINE FString GetEnumDisplayNameAsString(const FString& Name, TEnum Value)
    {
        const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
        if (!enumPtr) return FString("Invalid");
        return enumPtr->GetDisplayNameTextByValue((int64)Value).ToString();
    }

    template <typename EnumType>
    static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
    {
        UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
        if (!Enum) {
            return EnumType(0);
        }
        return (EnumType)Enum->FindEnumIndex(FName(*String));
    }
};
