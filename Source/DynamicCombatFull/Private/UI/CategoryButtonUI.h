// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "CategoryButtonUI.generated.h"

class UInventoryUI;
/**
 * 
 */
UCLASS()
class UCategoryButtonUI : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    void OnClicked();
    void OnHovered();
    void OnUnhovered();

public:
    void SetActiveBorder(bool bVisible);

    EItemType GetItemType() const { return  ItemType; }

private:
    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    UPROPERTY(EditAnywhere)
        UTexture2D* CategoryTexture;

    float SlotSize;

    UPROPERTY()
    UInventoryUI* InventoryUI;

    UPROPERTY(meta = (BindWidget))
        class UBorder* ActiveBorder;

    UPROPERTY(meta = (BindWidget))
        class UImage* CategoryImage;

    UPROPERTY(meta = (BindWidget))
        class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
        class USizeBox* SlotSizeBox;
};
