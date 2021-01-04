// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCore/DataTypes.h"
#include "InputBufferComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBufferConsumedSignature, EInputBufferKey, InKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferOpenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferCloseSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputBufferComponent();

protected:

public:	
    UFUNCTION(BlueprintCallable)
    void OpenInputBuffer();

    UFUNCTION(BlueprintCallable)
    void CloseInputBuffer();

public:
    void UpdateKey(EInputBufferKey InKey);
    EInputBufferKey GetStoredKey() const { return StoredKey; }

private:
    void ConsumeInputBuffer();

public:
    UPROPERTY(BlueprintAssignable)
    FInputBufferConsumedSignature OnInputBufferConsumed;

    UPROPERTY(BlueprintAssignable)
    FInputBufferOpenSignature OnInputBufferOpen;

    UPROPERTY(BlueprintAssignable)
    FInputBufferCloseSignature OnInputBufferClose;

private:
    EInputBufferKey StoredKey;

    bool bIsOpen;
};
