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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemClickedSignature, UInventoryItemUI*, Item);


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

    UFUNCTION()
    void OnItemAdded(FStoredItem InItem);

    UFUNCTION()
    void OnItemRemoved(FStoredItem InItem);

public:
    void InventoryItemClicked(UInventoryItemUI* InItem);
    void CreateItemWidgets();
    void UpdateItemWidgets(EItemType InType);
    bool IsOpen() const;

public:
    UPROPERTY(BlueprintAssignable)
        FInventoryItemClickedSignature OnInventoryItemClicked;

private:
    UPROPERTY(EditAnywhere)
        TSubclassOf<UInventoryItemUI> InventoryItemUIClass;

    UPROPERTY()
        class UInventoryComponent* InventoryComponent;

    UPROPERTY()
        class UEquipmentComponent* EquipmentComponent;


    TArray<UInventoryItemUI*> ItemWidgets;
    int GridColumns;
    EItemType DisplayedType;

    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ItemsGrid;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ItemsScrollBox;
};
