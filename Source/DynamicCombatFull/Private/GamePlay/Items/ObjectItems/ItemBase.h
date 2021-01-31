// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameCore/CustomStructs.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UItemBase : public UObject
{
	GENERATED_BODY()

public:
    virtual void BeginDestroy() override;

    virtual void UseItem(AActor* InActor) {}

    const FItem& GetItem() const { return Item; }

protected:
    UPROPERTY(EditAnywhere, Category = "Details")
    FItem Item;
};
