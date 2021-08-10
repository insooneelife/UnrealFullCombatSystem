// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TestPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UTestPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString ItemName;

	UPROPERTY(EditAnywhere)
		TArray<FString> Items;

	UPROPERTY(EditAnywhere)
		FColor ItemColor;

	UPROPERTY(EditAnywhere)
		FVector Loc;

	UPROPERTY(EditAnywhere)
		FQuat Rot;
};
