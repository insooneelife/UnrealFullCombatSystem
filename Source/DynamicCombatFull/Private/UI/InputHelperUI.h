// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputHelperUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UInputHelperUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    void Init(FText InKey, FText InAction);

    void UpdateInputHelper(FText InKey, FText InAction);

private:
    UPROPERTY(EditAnywhere)
    FText Key;

    UPROPERTY(EditAnywhere)
    FText Action;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HelperActionText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HelperKeyText;
};
