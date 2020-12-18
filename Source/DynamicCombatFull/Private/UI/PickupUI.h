// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupUI.generated.h"


class UInputHelpersUI;
class UTextBlock;
class UScrollBox;
class UPickupItemUI;
class UButton;
class APickupActor;


/**
 * 
 */
UCLASS()
class UPickupUI : public UUserWidget
{
	GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    void Close();
    void OnClickedTakeAllButton();
    void OnClickedCloseButton();

public:
    void CreateItemWidgets();
    void PickupItemClicked(UPickupItemUI* Item);

private:
    UPROPERTY()
        TSubclassOf<UPickupItemUI> PickupItemUIClass;


    UPROPERTY()
        class UInventoryComponent* InventoryComponent;

    FKey BackKey;

    FKey TakeAllKey;

    UPROPERTY(meta = (BindWidget))
        UButton* CloseButton;

    UPROPERTY(meta = (BindWidget))
        UInputHelpersUI* InputHelpers;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* PickupNameText;

    UPROPERTY(meta = (BindWidget))
        UScrollBox* PickupScrollBox;

    UPROPERTY(meta = (BindWidget))
        UPickupItemUI* Sample1;

    UPROPERTY(meta = (BindWidget))
        UPickupItemUI* Sample2;

    UPROPERTY(meta = (BindWidget))
        UButton* TakeAllButton;

    UPROPERTY()
        APickupActor* Pickup;

};
