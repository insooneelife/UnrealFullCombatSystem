// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatBarsUI.h"
#include "UI/StatBarUI.h"

void UAIStatBarsUI::NativeConstruct()
{
    Super::NativeConstruct();
    Stamina->SetVisibility(ESlateVisibility::Collapsed);
}


