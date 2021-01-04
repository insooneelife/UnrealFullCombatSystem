// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferComponent.h"

// Sets default values for this component's properties
UInputBufferComponent::UInputBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInputBufferComponent::OpenInputBuffer()
{
    bIsOpen = true;
    OnInputBufferOpen.Broadcast();
}

void UInputBufferComponent::CloseInputBuffer()
{
    bIsOpen = false;
    OnInputBufferClose.Broadcast();
}

void UInputBufferComponent::ConsumeInputBuffer()
{
    OnInputBufferConsumed.Broadcast(StoredKey);
    StoredKey = EInputBufferKey::None;
}

void UInputBufferComponent::UpdateKey(EInputBufferKey InKey)
{
    StoredKey = InKey;
    if (!bIsOpen)
    {
        ConsumeInputBuffer();
    }
}