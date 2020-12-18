// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockIconUI.generated.h"

/**
 * 
 */
UCLASS()
class ULockIconUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

private:

    UPROPERTY(meta = (BindWidget))
    class UImage* LockImage;
};
