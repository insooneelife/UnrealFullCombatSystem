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

    void Initialize(UArrowComponent* InArrowComp);
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

private:
    UPROPERTY()
    UArrowComponent* ArrowComponent;

    float AxisInputSensitivity;
    float TargetingMaxDistance;
    float TargetingMaxHeight;
    float TraceDepthOffset;
    float TraceHeightOffset;
    TArray<TEnumAsByte<EObjectTypeQuery>> AllowedCollisionTypes;
    TArray<TEnumAsByte<EObjectTypeQuery>> BlockingCollisionTypes;
    float DisableOnBlockDelay;
    EDrawDebugTrace::Type DebugMode;
    AActor* SelectedActor;
    bool bIsTargetingEnabled;
    bool bDebug;
    ACharacter* CharacterReference;
    FTimerHandle CheckTargetTimerHandle;
    bool bIsFreeCamera;
    FTimerHandle DisableCameraLockTimerHandle;

		
};
