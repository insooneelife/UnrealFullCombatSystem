// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputHelpersUI.generated.h"

class UHorizontalBox;
class UInputHelperUI;
/**
 * 
 */
UCLASS()
class UInputHelpersUI : public UUserWidget
{
	GENERATED_BODY()
public:
    UInputHelpersUI(const FObjectInitializer& ObjectInitializer);

protected:
public:
    UInputHelperUI* AddInputHelper(FText InKey, FText InAction);
    void ClearInputHelpers();
    void SetHorizontalAlignment(EHorizontalAlignment HorizontalAlignment);

private:

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InputHelpersHorizontalBox;


    UPROPERTY(EditAnywhere, Category = "LoadedClass")
    TSubclassOf<UInputHelperUI> InputHelperUIClass;
};
