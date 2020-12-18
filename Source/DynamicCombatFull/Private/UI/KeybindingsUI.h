// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeybindingsUI.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class UKeybindingsUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:

    virtual void NativeConstruct() override;

public:
    void ShowKeybindings();
    void HideKeybindings();


private:
    UPROPERTY(meta = (BindWidget))
    UBorder* Keybindings;
};
