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

    UFUNCTION()
    void OnItemInSlotChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex);

    UFUNCTION()
    void OnActiveItemChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex);

public:
    void UpdateWidget(FStoredItem InItem);
    void UpdateAmountText();
    void UpdateImage();
    void SetActiveWidget(bool bInVisible);

private:

    UPROPERTY()
    UEquipmentComponent* EquipmentComponent;

    UPROPERTY()
    UEquipmentUI* EquipmentUI;

    float SlotSize;

    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    UPROPERTY(EditAnywhere)
    int SlotIndex;

    UPROPERTY(EditAnywhere)
    int ItemIndex;

    FStoredItem Item;

    UPROPERTY(EditAnywhere)
        bool bShowActiveBorder;

    UPROPERTY(EditAnywhere)
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
