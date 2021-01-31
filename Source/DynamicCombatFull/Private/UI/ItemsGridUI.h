// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "ItemsGridUI.generated.h"

class UInventoryItemUI;
class UUniformGridPanel;
class UScrollBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemClickedSignature, UInventoryItemUI* const, Item);


/**
 * 
 */
UCLASS()
class UItemsGridUI : public UUserWidget
{
	GENERATED_BODY()
public:
    UItemsGridUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnItemAdded(const FStoredItem& InItem);

    UFUNCTION()
    void OnItemRemoved(const FStoredItem& InItem);

public:
    void InventoryItemClicked(UInventoryItemUI* const InItem);
    void CreateItemWidgets();
    void UpdateItemWidgets(EItemType InType);
    bool IsOpen() const;

public:
    UPROPERTY(BlueprintAssignable)
        FInventoryItemClickedSignature OnInventoryItemClicked;

private:

    TWeakObjectPtr<class UInventoryComponent> InventoryComponent;

    TWeakObjectPtr<class UEquipmentComponent> EquipmentComponent;

    TArray<UInventoryItemUI*> ItemWidgets;
    int GridColumns;
    EItemType DisplayedType;

    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<UInventoryItemUI> InventoryItemUIClass;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ItemsGrid;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ItemsScrollBox;
};
