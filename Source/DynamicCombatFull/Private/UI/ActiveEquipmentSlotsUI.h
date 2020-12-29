// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "ActiveEquipmentSlotsUI.generated.h"

class UActiveEquipmentSlotUI;
/**
 * 
 */
UCLASS()
class UActiveEquipmentSlotsUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
       

private:

    UPROPERTY(meta = (BindWidget))
        class UActiveEquipmentSlotUI* ToolSlot;

    UPROPERTY(meta = (BindWidget))
        class UActiveEquipmentSlotUI* ShieldSlot;

    UPROPERTY(meta = (BindWidget))
        class UActiveEquipmentSlotUI* ArrowsSlot;

    UPROPERTY(meta = (BindWidget))
        class UActiveEquipmentSlotUI* RangeWeaponSlot;

    UPROPERTY(meta = (BindWidget))
        class UActiveEquipmentSlotUI* SpellSlot;

    UPROPERTY(meta = (BindWidget))
        class UActiveEquipmentSlotUI* MeleeWeaponSlot;
};
