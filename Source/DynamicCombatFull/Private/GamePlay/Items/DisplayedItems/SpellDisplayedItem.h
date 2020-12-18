// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"
#include "SpellDisplayedItem.generated.h"

class UParticleSystem;
class UParticleSystemComponent;
/**
 * 
 */
UCLASS()
class ASpellDisplayedItem : public ADisplayedItem
{
	GENERATED_BODY()
	
public:
    ASpellDisplayedItem();

public:

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual bool Attach() override;

private:

    UPROPERTY(EditAnywhere)
    UParticleSystemComponent* ParticleSystemComponent;

    UPROPERTY()
    UParticleSystem* HandParticle;
};
