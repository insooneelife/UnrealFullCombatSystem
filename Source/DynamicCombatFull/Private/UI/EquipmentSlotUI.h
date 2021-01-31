// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "EquipmentSlotUI.generated.h"

class UEquipmentComponent;
class UEquipmentUI;
/**
 * 
 */
UCLASS()
class UEquipmentSlotUI : public UUserWidget
{
	GENERATED_BODY()
public:
    UEquipmentSlotUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnItemInSlotChanged(
        const FStoredItem& OldItem, const FStoredItem& NewItem, EItemType InType, int InSlotIndex, int InItemIndex);

    UFUNCTION()
    void OnActiveItemChanged(
        const FStoredItem& OldItem, const FStoredItem& NewItem, EItemType InType, int InSlotIndex, int InItemIndex);

    UFUNCTION()
    void OnClicked_SlotButton();

    UFUNCTION()
    void OnHovered_SlotButton();

    UFUNCTION()
    void OnUnhovered_SlotButton();

public:
    void UpdateWidget(const FStoredItem& InItem);
    void UpdateAmountText();
    void UpdateImage();
    void SetActiveWidget(bool bVisible);

public:

    EItemType GetItemType() const { return ItemType; }

    int GetSlotIndex() const { return SlotIndex; }

    int GetItemIndex() const { return ItemIndex; }

    const FStoredItem& GetItem() const { return Item; }

private:

    TWeakObjectPtr<UEquipmentComponent> EquipmentComponent;

    TWeakObjectPtr<UEquipmentUI> EquipmentUI;

    float SlotSize;

    FStoredItem Item;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UEquipmentUI> EquipmentUIClass;

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
    class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    class USizeBox* SlotSizeBox;
};
