// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUI.generated.h"

class UActiveEquipmentSlotsUI;
class UImage;
class UStatBarUI;
class UEquipmentUI;
class UEquipmentSlotsUI;
class UInteractionMessageUI;
class UInventoryUI;
/**
 * 
 */
UCLASS()
class UInGameUI : public UUserWidget
{
	GENERATED_BODY()
	

protected:


public:


private:

    UPROPERTY(meta = (BindWidget))
    UActiveEquipmentSlotsUI* ActiveSlots;

    UPROPERTY(meta = (BindWidget))
    UImage* Crosshair;

    UPROPERTY(meta = (BindWidget))
    UEquipmentUI* Equipment;

    UPROPERTY(meta = (BindWidget))
    UEquipmentSlotsUI* EquipmentSlots;

    UPROPERTY(meta = (BindWidget))
    UStatBarUI* Health;

    UPROPERTY(meta = (BindWidget))
    UInteractionMessageUI* InteractionMessage;

    UPROPERTY(meta = (BindWidget))
    UInventoryUI* Inventory;

    UPROPERTY(meta = (BindWidget))
    UStatBarUI* Mana;

    UPROPERTY(meta = (BindWidget))
    UStatBarUI* Stamina;


};
