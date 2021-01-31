// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMessageUI.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "InputHelperUI.h"
#include "InputHelpersUI.h"
#include "GameCore/DefaultGameInstance.h"

void UInteractionMessageUI::NativeConstruct()
{
    Super::NativeConstruct();
    InteractInputHelper = InputHelpers->AddInputHelper(FText(), FText());

    UpdateWidget(NAME_None);
    InputHelpers->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
}

void UInteractionMessageUI::UpdateWidget(FName InMessage)
{
    if (InMessage == NAME_None)
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        SetVisibility(ESlateVisibility::Visible);

        FKey Key = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::Interact);
        FText KeyDisplayName = UKismetInputLibrary::Key_GetDisplayName(Key);
        FText MessageText = UKismetTextLibrary::Conv_NameToText(InMessage);

        if (InteractInputHelper.IsValid())
        {
            InteractInputHelper->UpdateInputHelper(KeyDisplayName, MessageText);
        }
    }
}
