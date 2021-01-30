// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHelpersUI.h"

#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/BorderSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "InputHelperUI.h"
#include "GameCore/GameUtils.h"

UInputHelpersUI::UInputHelpersUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer)
{
    //static TSubclassOf<UInputHelperUI> LoadedClass =
    //    GameUtils::LoadAssetClass<UInputHelperUI>("/Game/DynamicCombatSystem/Widgets/InputHelperWB");

    //InputHelperUIClass = LoadedClass;
}

UInputHelperUI* UInputHelpersUI::AddInputHelper(FText InKey, FText InAction)
{
    UInputHelperUI* CreatedUI = Cast<UInputHelperUI>(CreateWidget(GetOwningPlayer(), InputHelperUIClass));
    CreatedUI->Init(InKey, InAction);

    InputHelpersHorizontalBox->AddChildToHorizontalBox(CreatedUI)->SetPadding(FMargin(30.0f, 0.0f, 0.0f, 0.0f));
    
    return CreatedUI;
}

void UInputHelpersUI::ClearInputHelpers()
{
    InputHelpersHorizontalBox->ClearChildren();
}

void UInputHelpersUI::SetHorizontalAlignment(EHorizontalAlignment HorizontalAlignment)
{
    UBorderSlot* BorderSlot = UWidgetLayoutLibrary::SlotAsBorderSlot(InputHelpersHorizontalBox);
    BorderSlot->SetHorizontalAlignment(HorizontalAlignment);
}
