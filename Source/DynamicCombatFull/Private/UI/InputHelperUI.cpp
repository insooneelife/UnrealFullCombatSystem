// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHelperUI.h"
#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "InputHelper"

void UInputHelperUI::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateInputHelper(Key, Action);
}

void UInputHelperUI::Init(FText InKey, FText InAction)
{
    Key = InKey;
    Action = InAction;
}

void UInputHelperUI::UpdateInputHelper(FText InKey, FText InAction)
{
    FFormatNamedArguments Arguments;
    Arguments.Add(TEXT("Key"), InKey);
    FText Result = FText::Format(LOCTEXT("KeyText", "[{Key}]"), Arguments);

    HelperKeyText->SetText(Result);
    HelperActionText->SetText(InAction);
}


#undef LOCTEXT_NAMESPACE