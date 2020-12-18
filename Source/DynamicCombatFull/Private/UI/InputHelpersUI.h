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
	
protected:
public:
    UInputHelperUI* AddInputHelper(FText InKey, FText InAction);
    void ClearInputHelpers();
    void SetHorizontalAlignment(EHorizontalAlignment HorizontalAlignment);

private:

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* InputHelpersHorizontalBox;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UInputHelperUI> CreateUIClass;
};
