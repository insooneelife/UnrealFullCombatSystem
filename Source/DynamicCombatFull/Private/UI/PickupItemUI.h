// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupItemUI.generated.h"

class UTextBlock;
class UButton;
class UImage;
class UPickupUI;
class UItemBase;
class APickupActor;

/**
 * 
 */
UCLASS()
class UPickupItemUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnClicked_ItemButton();

public:
    void UpdateWidget();

    void Init(
        UPickupUI* const InPickupUI,
        TSubclassOf<UItemBase> InItemClass,
        int InItemAmount,
        APickupActor* const InPickup);

public:

    TSubclassOf<UItemBase> GetItemClass() const { return ItemClass; }

private:

    UPROPERTY(meta = (BindWidget))
        UTextBlock* AmountText;

    UPROPERTY(meta = (BindWidget))
        UButton* ItemButton;

    UPROPERTY(meta = (BindWidget))
        UImage* ItemImage;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* NameText;

    UPROPERTY(EditAnywhere)
        UPickupUI* PickupUI;

    UPROPERTY(EditAnywhere)
        TSubclassOf<UItemBase> ItemClass;

    UPROPERTY(EditAnywhere)
        int ItemAmount;

    UPROPERTY(EditAnywhere)
        APickupActor* Pickup;
};
