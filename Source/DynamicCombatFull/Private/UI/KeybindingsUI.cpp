// Fill out your copyright notice in the Description page of Project Settings.


#include "KeybindingsUI.h"
#include "Components/Border.h"

void UKeybindingsUI::NativeConstruct()
{
    HideKeybindings();
}

void UKeybindingsUI::ShowKeybindings()
{
    Keybindings->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UKeybindingsUI::HideKeybindings()
{
    Keybindings->SetVisibility(ESlateVisibility::Hidden);
}
