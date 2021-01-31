// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionMessageUI.generated.h"

class UInputHelperUI;
class UInputHelpersUI;
/**
 * 
 */
UCLASS()
class UInteractionMessageUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    void UpdateWidget(FName InMessage);

private:

    UPROPERTY(meta = (BindWidget))
    UInputHelpersUI* InputHelpers;

    TWeakObjectPtr<UInputHelperUI> InteractInputHelper;
};
