// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "InventoryUI.generated.h"

class UInventoryItemUI;
class UCategoryButtonUI;
class UInputHelpersUI;
class UItemsGridUI;
class UItemActionsUI;

/**
 * 
 */
UCLASS()
class UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

    UFUNCTION()
    void FocusSelf();

    UFUNCTION()
    void OnInventoryItemClicked(UInventoryItemUI* InItem);

    UFUNCTION()
    void OnClicked_CloseButton();

public:
    void Open();
    void Close();

    void CategoryButtonClicked(UCategoryButtonUI* Button);
    void SetCategoryButton(UCategoryButtonUI* Button);

    EItemType GetDisplayedType() const;

private:

    UPROPERTY(EditAnywhere)
        TSubclassOf<UItemActionsUI> ItemActionsUIClass;

    UPROPERTY()
        class UInventoryComponent* InventoryComponent;

    UPROPERTY()
        class UEquipmentComponent* EquipmentComponent;


    FKey InventoryKey;
    FKey BackKey;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* AllItemsCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* ArrowsCategoryButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* CloseButton;

    UPROPERTY(meta = (BindWidget))
        class UVerticalBox* EquipmentVerticalBox;


    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* FeetCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* HandsCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* HeadCategoryButton;

    UPROPERTY(meta = (BindWidget))
    UInputHelpersUI* InputHelpers;

    UPROPERTY(meta = (BindWidget))
    UItemsGridUI* InventoryItemsGrid;

    UPROPERTY(meta = (BindWidget))
        class USizeBox* InventorySizeBox;


    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* LegsCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* MaterialCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* MeleeWeaponCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* NecklaceCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* RangeWeaponCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* RingCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* ShieldCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* SpellCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* ToolCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* TopCategoryButton;

    UPROPERTY(meta = (BindWidget))
        UCategoryButtonUI* SelectedCategoryButton;

};
