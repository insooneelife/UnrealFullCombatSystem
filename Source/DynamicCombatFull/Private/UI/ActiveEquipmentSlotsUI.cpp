// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEquipmentSlotsUI.h"

void UActiveEquipmentSlotsUI::Init(
    UActiveEquipmentSlotUI* InToolSlot,
    UActiveEquipmentSlotUI* InShieldSlot,
    UActiveEquipmentSlotUI* InArrowsSlot,
    UActiveEquipmentSlotUI* InRangeWeaponSlot,
    UActiveEquipmentSlotUI* InSpellSlot,
    UActiveEquipmentSlotUI* InMeleeWeaponSlot)
{
    ToolSlot = InToolSlot;
    ShieldSlot = InShieldSlot;
    ArrowsSlot = InArrowsSlot;
    RangeWeaponSlot = InRangeWeaponSlot;
    SpellSlot = InSpellSlot;
    MeleeWeaponSlot = InMeleeWeaponSlot;
}