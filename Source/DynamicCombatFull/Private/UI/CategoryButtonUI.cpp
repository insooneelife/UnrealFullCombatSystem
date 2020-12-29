// Fill out your copyright notice in the Description page of Project Settings.


#include "CategoryButtonUI.h"

#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "InventoryUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameCore/GameUtils.h"

UCategoryButtonUI::UCategoryButtonUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer), SlotSize(96.0f)
{
    static UTexture2D* LoadedObject =
        GameUtils::LoadAssetObject<UTexture2D>(TEXT("/Game/DynamicCombatSystem/Widgets/Textures/T_MeleeWeapon"));

    CategoryTexture = LoadedObject;
}

void UCategoryButtonUI::NativePreConstruct()
{
    Super::NativePreConstruct();

    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
    CategoryImage->SetBrushFromTexture(CategoryTexture);
}

void UCategoryButtonUI::NativeConstruct()
{
    Super::NativeConstruct();

    SlotButton->OnClicked.AddDynamic(this, &UCategoryButtonUI::OnClicked_SlotButton);
    SlotButton->OnHovered.AddDynamic(this, &UCategoryButtonUI::OnHovered_SlotButton);
    SlotButton->OnUnhovered.AddDynamic(this, &UCategoryButtonUI::OnUnhovered_SlotButton);
}

void UCategoryButtonUI::NativeDestruct()
{
    SlotButton->OnClicked.RemoveDynamic(this, &UCategoryButtonUI::OnClicked_SlotButton);
    SlotButton->OnHovered.RemoveDynamic(this, &UCategoryButtonUI::OnHovered_SlotButton);
    SlotButton->OnUnhovered.RemoveDynamic(this, &UCategoryButtonUI::OnUnhovered_SlotButton);

    Super::NativeDestruct();
}

void UCategoryButtonUI::OnClicked_SlotButton()
{
    if (InventoryUI->IsValidLowLevel())
    {
        InventoryUI->CategoryButtonClicked(this);
    }
}

void UCategoryButtonUI::OnHovered_SlotButton()
{
    CategoryImage->SetColorAndOpacity(GameUtils::Brown);
}

void UCategoryButtonUI::OnUnhovered_SlotButton()
{
    CategoryImage->SetColorAndOpacity(FLinearColor::Gray);
}

// called after NativeConstruct
void UCategoryButtonUI::Init(UInventoryUI* InInventoryUI)
{
    InventoryUI = InInventoryUI;
    if (InventoryUI->IsValidLowLevel())
    {
        if (ItemType == InventoryUI->GetDisplayedType())
        {
            InventoryUI->SetCategoryButton(this);
        }
    }
}

void UCategoryButtonUI::SetActiveBorder(bool bVisible)
{
    FLinearColor Color = bVisible ? GameUtils::Brown : GameUtils::Gray;
    ActiveBorder->SetBrushColor(Color);
}
