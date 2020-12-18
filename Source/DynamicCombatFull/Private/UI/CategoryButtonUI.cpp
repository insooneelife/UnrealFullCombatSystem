// Fill out your copyright notice in the Description page of Project Settings.


#include "CategoryButtonUI.h"

#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "InventoryUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameCore/GameUtils.h"

void UCategoryButtonUI::NativePreConstruct()
{
    SlotSizeBox->SetWidthOverride(SlotSize);
    SlotSizeBox->SetHeightOverride(SlotSize);
    CategoryImage->SetBrushFromTexture(CategoryTexture);
}

void UCategoryButtonUI::NativeConstruct()
{
    SlotButton->OnClicked.AddDynamic(this, &UCategoryButtonUI::OnClicked);
    SlotButton->OnHovered.AddDynamic(this, &UCategoryButtonUI::OnHovered);
    SlotButton->OnUnhovered.AddDynamic(this, &UCategoryButtonUI::OnUnhovered);

    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UInventoryUI::StaticClass());

    InventoryUI = Cast<UInventoryUI>(FoundWidgets[0]);
    if (InventoryUI->IsValidLowLevel())
    {
        if (ItemType == InventoryUI->GetDisplayedType())
        {
            InventoryUI->SetCategoryButton(this);
        }
    }
}

void UCategoryButtonUI::OnClicked()
{
    if (InventoryUI->IsValidLowLevel())
    {
        InventoryUI->CategoryButtonClicked(this);
    }
}

void UCategoryButtonUI::OnHovered()
{
    CategoryImage->SetColorAndOpacity(GameUtils::Brown);
}

void UCategoryButtonUI::OnUnhovered()
{
    CategoryImage->SetColorAndOpacity(FLinearColor::Gray);
}

void UCategoryButtonUI::SetActiveBorder(bool bVisible)
{
    FLinearColor Color = bVisible ? GameUtils::Brown : GameUtils::Gray;
    ActiveBorder->SetBrushColor(Color);
}
