// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBarUI.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "GameCore/DefaultGameInstance.h"
#include "GameCore/GameUtils.h"
#include "Components/ExtendedStatComponent.h"

UStatBarUI::UStatBarUI(const FObjectInitializer& ObjectInitializer)
    :
    Super(ObjectInitializer),
    FillColor(0.3f, 0.0f, 0.0f, 1.0f)
{
}

void UStatBarUI::NativePreConstruct()
{
    Super::NativePreConstruct();

    SetFillColor(FillColor);
}

void UStatBarUI::NativeConstruct()
{
    Super::NativeConstruct();
}

void UStatBarUI::NativeDestruct()
{
    Super::NativeDestruct();
}

void UStatBarUI::OnValueChanged(float InNewValue, float InMaxValue)
{
    CurrentValue = InNewValue;
    MaxValue = InMaxValue;

    float Percent = FMath::Clamp(InNewValue / InMaxValue, 0.0f, 1.0f);

    UpdatePercent(Percent);
    UpdateWidth(InMaxValue);
}

void UStatBarUI::Init(UExtendedStatComponent* InStatComp)
{
    if (GameUtils::IsValid(InStatComp))
    {
        OnValueChanged(InStatComp->GetCurrentValue(), InStatComp->GetMaxValue());
        InStatComp->OnValueChanged.AddDynamic(this, &UStatBarUI::OnValueChanged);
    }
}

void UStatBarUI::UpdatePercent(float InPercent)
{
    StatProgressBar->SetPercent(InPercent);
}

void UStatBarUI::SetFillColor(FLinearColor InColor)
{
    StatProgressBar->SetFillColorAndOpacity(InColor);
}

void UStatBarUI::UpdateWidth(float InMaxValue)
{
    if (GameUtils::IsValid(GetOwningPlayer()))
    {
        UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(StatSizeBox);
        float MaxClampedValue;

        if (CanvasSlot != nullptr)
        {
            MaxClampedValue = 
                UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X - CanvasSlot->GetPosition().X * 2.0f;
        }
        else
        {
            MaxClampedValue = UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X - 200.0f;
        }

        float FinalMaxValue = MaxClampedValue >= 0.0f ? MaxClampedValue : 1000.0f;
        float Width = FMath::Clamp(InMaxValue * 3.0f, 200.0f, FinalMaxValue);

        StatSizeBox->SetWidthOverride(Width);
    }
}
