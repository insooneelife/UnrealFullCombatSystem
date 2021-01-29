// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameCore/DataTypes.h"
#include "GameCore/CustomStructs.h"
#include "DynamicTargetingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetChangedSignature, AActor* const, InNewTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetingToggledSignature, bool, bInEnabled);

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

public:	
	// Called every frame
	virtual void TickComponent(
        float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void Init(UArrowComponent* const InArrowComp);

    void ToggleCameraLock();
    void FindTargetOnLeft();
    void FindTargetOnRight();
    void FindTargetWithAxisInput(float InAxisValue);
    void SetFreeCamera(bool bInFreeCamera);
    void DisableCameraLock();

    FTransform GetSelectedActorTransform() const;
    bool IsTargetingEnabled() const;
    void SetBlockingCollisionTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& InBlockingCollisionTypes)
    {
        BlockingCollisionTypes = InBlockingCollisionTypes;
    }

private:

    void UpdateTarget();
    void FindTarget();
    void FindDirectionalTarget(bool bInOnLeft);
    void EnableCameraLock();
    void UpdateCameraLock();
    void SetDebugMode();
    void UpdateIgnoreLookInput();
    void UpdateCollisionTypeArrays();

    bool IsInViewport(FVector2D InScreenPosition) const;
    FVector GetLineTraceStartLocation()const;

    AActor* GetTargetByDotProduct(const TArray<AActor*>& InActors, bool bInBest) const;
    bool IsAnythingBlockingTrace(const AActor* const InTarget, const TArray<AActor*>& InActorsToIgnore) const;

    bool GetActorScreenPosition(const AActor* const InActor, FVector2D& OutScreenPos) const;
    float GetDistanceToOwner(const AActor* const InOtherActor) const;
    bool IsTargetRightSide(const AActor* const InPotentialTarget) const;
    float CalculateDotProductToTarget(const AActor* const InTarget) const;
    bool HasArrayAnyElem(const TArray<AActor*>& InActors) const;

public:
    UPROPERTY(BlueprintAssignable)
    FTargetChangedSignature OnTargetChanged;

    UPROPERTY(BlueprintAssignable)
    FTargetingToggledSignature OnTargetingToggled;

private:
    TWeakObjectPtr<UArrowComponent> ArrowComponent;

    TWeakObjectPtr<AActor> SelectedActor;

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

    UPROPERTY(EditAnywhere)
    bool bDebug;

    EDrawDebugTrace::Type DebugMode;

    bool bIsTargetingEnabled;
    bool bIsFreeCamera;
    FTimerHandle CheckTargetTimerHandle;
    FTimerHandle DisableCameraLockTimerHandle;

		
};
