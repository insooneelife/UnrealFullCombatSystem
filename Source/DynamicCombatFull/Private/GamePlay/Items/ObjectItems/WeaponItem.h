// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Items/ObjectItems/ItemBase.h"
#include "GameCore/DataTypes.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class UWeaponItem : public UItemBase
{
	GENERATED_BODY()

public:
    EWeaponType GetWeaponType() const { return WeaponType; }

protected:
    UPROPERTY(EditAnywhere)
    EWeaponType WeaponType;
};
