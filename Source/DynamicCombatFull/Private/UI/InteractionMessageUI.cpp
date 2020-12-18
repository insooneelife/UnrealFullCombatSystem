// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMessageUI.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "InputHelperUI.h"
#include "InputHelpersUI.h"
#include "GameCore/DefaultGameInstance.h"

const FName UInteractionMessageUI::None = FName(TEXT("None"));

void UInteractionMessageUI::NativeConstruct()
{
    Super::NativeConstruct();
    InteractInputHelper = InputHelpers->AddInputHelper(FText(), FText());

    UpdateWidget(None);
    InputHelpers->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
}

void UInteractionMessageUI::UpdateWidget(FName InMessage)
{
    if (InMessage == None)
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        SetVisibility(ESlateVisibility::Visible);

        FKey Key = UDefaultGameInstance::GetFirstActionMappingKey(UDefaultGameInstance::Interact);
        FText KeyDisplayName = UKismetInputLibrary::Key_GetDisplayName(Key);
        FText MessageText = UKismetTextLibrary::Conv_NameToText(InMessage);

        InteractInputHelper->UpdateInputHelper(KeyDisplayName, MessageText);
    }
}
