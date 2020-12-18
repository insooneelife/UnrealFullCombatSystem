// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AIStatBarsUI.generated.h"

/**
 * 
 */
UCLASS()
class UAIStatBarsUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:

    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
        class UStatBarUI* Stamina;

    UPROPERTY(meta = (BindWidget))
        class UStatBarUI* Health;
};
