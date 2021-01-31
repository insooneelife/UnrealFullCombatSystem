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

public:
    UCategoryButtonUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnClicked_SlotButton();

    UFUNCTION()
    void OnHovered_SlotButton();

    UFUNCTION()
    void OnUnhovered_SlotButton();

public:
    void Init(UInventoryUI* const InInventoryUI);
    void SetActiveBorder(bool bInVisible);

    EItemType GetItemType() const { return  ItemType; }

private:
    float SlotSize;

    TWeakObjectPtr<UInventoryUI> InventoryUI;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UInventoryUI> InventoryUIClass;

    UPROPERTY(EditAnywhere)
    EItemType ItemType;

    UPROPERTY(EditAnywhere, Category = "LoadedObject")
    UTexture2D* CategoryTexture;

    UPROPERTY(meta = (BindWidget))
    class UBorder* ActiveBorder;

    UPROPERTY(meta = (BindWidget))
    class UImage* CategoryImage;

    UPROPERTY(meta = (BindWidget))
    class UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    class USizeBox* SlotSizeBox;
};
