// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentUI.generated.h"

class UInputHelpersUI;
class UItemsGridUI;
class UInventoryItemUI;
class UEquipmentSlotUI;
/**
 * 
 */
UCLASS()
class UEquipmentUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    UFUNCTION()
    void OnInventoryItemClicked(UInventoryItemUI* const InItem);

    UFUNCTION()
    void OnClicked_CloseButton();

public:
    void Open();
    void Close();

    void EquipmentSlotClicked(UEquipmentSlotUI* const InSlot);
    void SetActiveWidget(int WidgetIndex);

private:
    TWeakObjectPtr<class UInventoryComponent> InventoryComponent;

    TWeakObjectPtr<class UEquipmentComponent> EquipmentComponent;

    TWeakObjectPtr<class UEquipmentSlotUI> ClickedSlot;

    FKey EquipmentKey;
    FKey BackKey;

    UPROPERTY(meta = (BindWidget))
    class UButton* CloseButton;

    UPROPERTY(meta = (BindWidget))
    class USizeBox* EquipmentSizeBox;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* EquipmentVerticalBox;

    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher* EquipmentWidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    class UInputHelpersUI* InputHelpers;

    UPROPERTY(meta = (BindWidget))
    class UItemsGridUI* InventoryItemsGrid;



};
