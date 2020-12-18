// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameCore/CustomStructs.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void UseItem(AActor* InActor) {}

    const FItem& GetItem() const { return Item; }

protected:
    UPROPERTY()
    FItem Item;
};
