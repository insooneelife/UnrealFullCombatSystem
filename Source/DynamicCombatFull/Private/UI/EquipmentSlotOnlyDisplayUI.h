// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "EquipmentSlotOnlyDisplayUI.generated.h"

class UEquipmentUI;
class UEquipmentComponent;
/**
 * 
 */
UCLASS()
class UEquipmentSlotOnlyDisplayUI : public UUserWidget
{
	GENERATED_BODY()
public:
    UEquipmentSlotOnlyDisplayUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnItemInSlotChanged(
        const FStoredItem& InOldItem, 
        const FStoredItem& InNewItem, 
        EItemType InType, 
        int InSlotIndex, 
        int InItemIndex);

    UFUNCTION()
    void OnActiveItemChanged(
        const FStoredItem& InOldItem,
        const FStoredItem& InNewItem, 
        EItemType InType,
        int InSlotIndex,
        int InItemIndex);

public:
    void UpdateWidget(const FStoredItem& InItem);
    void UpdateAmountText();
    void UpdateImage();
    void SetActiveWidget(bool bInVisible);

private:

    TWeakObjectPtr<UEquipmentComponent> EquipmentComponent;

    float SlotSize;

    FStoredItem Item;

    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    UPROPERTY(EditAnywhere)
    int SlotIndex;

    UPROPERTY(EditAnywhere)
    int ItemIndex;

    UPROPERTY(EditAnywhere)
    bool bShowActiveBorder;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UTexture2D* BackgroundTexture;

    UPROPERTY(meta = (BindWidget))
    class UBorder* ActiveBorder;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* AmountText;

    UPROPERTY(meta = (BindWidget))
        class UImage* BackgroundImage;

    UPROPERTY(meta = (BindWidget))
        class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
        class USizeBox* SlotSizeBox;

};
