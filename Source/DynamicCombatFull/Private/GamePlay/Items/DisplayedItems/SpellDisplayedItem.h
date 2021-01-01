// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "HandDisplayedItem.h"
#include "SpellDisplayedItem.generated.h"

class UParticleSystem;
class UParticleSystemComponent;
/**
 * 
 */
UCLASS()
class ASpellDisplayedItem : public AHandDisplayedItem
{
	GENERATED_BODY()
	
public:
    ASpellDisplayedItem();

public:
    virtual bool Attach() override;

private:

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* ParticleSystemComponent;
};
