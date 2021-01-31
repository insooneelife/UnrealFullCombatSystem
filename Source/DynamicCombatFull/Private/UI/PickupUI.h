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

public:
    UPickupUI(const FObjectInitializer& ObjectInitializer);

protected:

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    void Close();

    UFUNCTION()
    void OnClicked_TakeAllButton();

    UFUNCTION()
    void OnClicked_CloseButton();

public:
    void CreateItemWidgets();
    void PickupItemClicked(UPickupItemUI* const InItem);

private:

    TWeakObjectPtr<class UInventoryComponent> InventoryComponent;

    TWeakObjectPtr<APickupActor> Pickup;

    FKey BackKey;

    FKey TakeAllKey;

    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<UPickupItemUI> PickupItemUIClass;

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

};
