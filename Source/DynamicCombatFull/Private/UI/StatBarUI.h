// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "StatBarUI.generated.h"

class UExtendedStatComponent;
/**
 * 
 */
UCLASS()
class UStatBarUI : public UUserWidget
{
	GENERATED_BODY()
public:
    UStatBarUI(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnValueChanged(float InNewValue, float InMaxValue);
    void Init(UExtendedStatComponent* InStatComp);

public:
    void UpdatePercent(float InPercent);
    void SetFillColor(FLinearColor InColor);
    void UpdateWidth(float InMaxValue);

private:
    UPROPERTY(EditAnywhere)
    EStat ExtendedStatType;

    UPROPERTY(EditAnywhere)
    FLinearColor FillColor;

    float CurrentValue;

    float MaxValue;


    UPROPERTY(meta = (BindWidget))
    class UProgressBar* StatProgressBar;

    UPROPERTY(meta = (BindWidget))
    class USizeBox* StatSizeBox;
};
