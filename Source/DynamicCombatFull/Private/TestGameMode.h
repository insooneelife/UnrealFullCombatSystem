#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestGameMode.generated.h"

class UTestPrimaryDataAsset;

UCLASS()
class ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UTestPrimaryDataAsset* DataAsset;
};
