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

    void Init(
        UActiveEquipmentSlotUI* InToolSlot,
        UActiveEquipmentSlotUI* InShieldSlot,
        UActiveEquipmentSlotUI* InArrowsSlot,
        UActiveEquipmentSlotUI* InRangeWeaponSlot,
        UActiveEquipmentSlotUI* InSpellSlot,
        UActiveEquipmentSlotUI* InMeleeWeaponSlot);

    UActiveEquipmentSlotUI* GetToolSlot() const { return ToolSlot; }
    UActiveEquipmentSlotUI* GetShieldSlot() const{ return ToolSlot; }
    UActiveEquipmentSlotUI* GetArrowsSlot() const{ return ToolSlot; }
    UActiveEquipmentSlotUI* GetRangeWeaponSlot() const { return ToolSlot; }
    UActiveEquipmentSlotUI* GetSpellSlot() const { return ToolSlot; }
    UActiveEquipmentSlotUI* GetMeleeWeaponSlot() const { return ToolSlot; }

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
