// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "DynamicTargetingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetChangedSignature, AActor*, NewTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetingToggledSignature, bool, bEnabled);

class ACharacter;
class UArrowComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDynamicTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void Init(UArrowComponent* InArrowComp);
    void DisableCameraLock();
    void FindTargetWithAxisInput(float AxisValue);
    void ToggleCameraLock();
    bool IsTargetingEnabled() const;
    void FindTargetOnLeft();
    void FindTargetOnRight();
    void SetFreeCamera(bool bFreeCamera);
    void FindTarget();
    bool IsInViewport(FVector2D ScreenPosition);
    FVector GetLineTraceStartLocation()const;
    void EnableCameraLock();
    void UpdateCameraLock();
    void SetDebugMode();
    void CheckTarget();
    void FindDirectionalTarget(bool bOnLeft);
    AActor* GetTargetByDotProduct(const TArray<AActor*> Actors, bool bBest) const;
    void UpdateIgnoreLookInput();
    bool IsAnythingBlockingTrace(AActor* Target, const TArray<AActor*>& ActorsToIgnore) const;
    void CheckCollisionTypeArrays();

    bool GetActorScreenPosition(AActor* InActor, FVector2D& OutScreenPos) const;
    float GetDistanceToOwner(AActor* OtherActor) const;
    bool IsTargetRightSide(AActor* PotentialTarget) const;
    float CalculateDotProductToTarget(AActor* Target) const;
    bool HasArrayAnyElem(const TArray<AActor*>& Actors) const;

public:
    UPROPERTY(BlueprintAssignable)
    FTargetChangedSignature OnTargetChanged;

    UPROPERTY(BlueprintAssignable)
    FTargetingToggledSignature OnTargetingToggled;

public:
    AActor* GetSelectedActor() const { return SelectedActor; }

    void SetBlockingCollisionTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& InBlockingCollisionTypes) 
    {
        BlockingCollisionTypes = InBlockingCollisionTypes;
    }

private:
    UPROPERTY()
    UArrowComponent* ArrowComponent;

    UPROPERTY(EditAnywhere)
    float AxisInputSensitivity;

    UPROPERTY(EditAnywhere)
    float TargetingMaxDistance;

    UPROPERTY(EditAnywhere)
    float TargetingMaxHeight;

    UPROPERTY(EditAnywhere)
    float TraceDepthOffset;

    UPROPERTY(EditAnywhere)
    float TraceHeightOffset;

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EObjectTypeQuery>> AllowedCollisionTypes;

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EObjectTypeQuery>> BlockingCollisionTypes;

    UPROPERTY(EditAnywhere)
    float DisableOnBlockDelay;


    EDrawDebugTrace::Type DebugMode;
    AActor* SelectedActor;
    bool bIsTargetingEnabled;

    UPROPERTY(EditAnywhere)
    bool bDebug;

    ACharacter* CharacterReference;
    FTimerHandle CheckTargetTimerHandle;
    bool bIsFreeCamera;
    FTimerHandle DisableCameraLockTimerHandle;

		
};
