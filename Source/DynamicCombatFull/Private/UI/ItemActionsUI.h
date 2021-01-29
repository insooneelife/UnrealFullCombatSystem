// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "ItemActionsUI.generated.h"

class UInventoryComponent;
class USizeBox;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetRemovedSignature);

/**
 * 
 */
UCLASS()
class UItemActionsUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    // called one tick after Spawn, so Init is called before this 
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    void Close();

    UFUNCTION()
    void OnClicked_UseButton();

    UFUNCTION()
    void OnClicked_DropButton();

    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
    void SetPosition(FVector2D Position);

    void Init(UInventoryComponent* const InInventoryComponent, FVector2D InSpawnPosition, const FStoredItem& InItem);

public:
    UPROPERTY(BlueprintAssignable)
        FWidgetRemovedSignature OnWidgetRemoved;

private:
    UPROPERTY()
    UInventoryComponent* InventoryComponent;

    FKey BackKey;

    UPROPERTY(meta = (BindWidget))
        class USizeBox* ActionsSB;

    UPROPERTY(meta = (BindWidget))
        class UButton* DropButton;

    UPROPERTY(meta = (BindWidget))
        class UButton* UseButton;


    UPROPERTY(EditAnywhere)
    FVector2D SpawnPosition;

    UPROPERTY(EditAnywhere)
    FStoredItem Item;
};
