// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "ActiveEquipmentSlotUI.generated.h"

/**
 * 
 */
UCLASS()
class UActiveEquipmentSlotUI : public UUserWidget
{
	GENERATED_BODY()
public:
    UActiveEquipmentSlotUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnActiveItemChanged(FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InItemIndex);

    UFUNCTION()
    void OnSlotHiddenChanged(EItemType InSlotType, int InSlotIndex, FStoredItem InActiveItem, bool bInIsHidden);

public:
    void UpdateWidget(FStoredItem InItem);
    UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
        void PlayBlinkBorderAnimation();
    void UpdateAmountText();
    void UpdateItemImage();
    bool SetIsHidden(bool bIsHidden);


private:

    UPROPERTY()
        class UInventoryComponent* InventoryComponent;

    UPROPERTY()
        class UEquipmentComponent* EquipmentComponent;

    UPROPERTY(EditAnywhere)
    EItemType SlotType;

    FStoredItem Item;

    UPROPERTY(EditAnywhere)
    FVector2D SlotSize;

    UPROPERTY(EditAnywhere)
    UTexture2D* BackgroundTexture;

    UPROPERTY(meta = (BindWidget))
        class UTextBlock* AmountText;

    UPROPERTY(meta = (BindWidget))
        class UImage* BackgroundImage;

    UPROPERTY(meta = (BindWidget))
        class UBorder* BorderCenter;

    UPROPERTY(meta = (BindWidget))
        class UImage* IsHiddenImage;

    UPROPERTY(meta = (BindWidget))
        class UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
        class USizeBox* SlotSizeBox;

};
