// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTargetingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Interfaces/IsTargetable.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UDynamicTargetingComponent::UDynamicTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    AxisInputSensitivity = 1.5f;
    TargetingMaxDistance = 2500.0f;
    TargetingMaxHeight = 640.0f;
    TraceHeightOffset = 100.0f;
    AllowedCollisionTypes = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)};
    DisableOnBlockDelay = 2.0f;
}

// Called every frame
void UDynamicTargetingComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateCameraLock();
}

void UDynamicTargetingComponent::Init(UArrowComponent* InArrowComp)
{
    ArrowComponent = InArrowComp;
    if(ArrowComponent.IsValid())
    {
        ArrowComponent->SetUsingAbsoluteRotation(true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ArrowComponent is not valid!"));
    }

    SetDebugMode();
    UpdateCollisionTypeArrays();
}

void UDynamicTargetingComponent::ToggleCameraLock()
{
    if (IsTargetingEnabled())
    {
        DisableCameraLock();
    }
    else
    {
        FindTarget();
    }
}

void UDynamicTargetingComponent::FindTargetOnLeft()
{
    return FindDirectionalTarget(true);
}

void UDynamicTargetingComponent::FindTargetOnRight()
{
    return FindDirectionalTarget(false);
}

void UDynamicTargetingComponent::FindTargetWithAxisInput(float InAxisValue)
{
    const float StartRotatingThreshold = 1.5f;

    if (ArrowComponent.IsValid() &&
        SelectedActor.IsValid() &&
        FMath::Abs(InAxisValue) > StartRotatingThreshold &&
        !bIsFreeCamera)
    {
        FRotator A = GetOwner()->GetInstigator()->GetControlRotation();
        FRotator B = ArrowComponent->GetComponentRotation();
        FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(A, B);

        if (FMath::Abs(Delta.Yaw) <= 165.0f)
        {
            float Yaw = InAxisValue * AxisInputSensitivity;
            float Val = UKismetSystemLibrary::MakeLiteralFloat(625.0f) * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
            float Min = -Val;
            float Max = Val;
            float ClampedYaw = FMath::Clamp(Yaw, Min, Max);
            ArrowComponent->AddLocalRotation(FRotator(0.0f, ClampedYaw, 0.0f));
        }

        FVector Start = ArrowComponent->GetComponentLocation();
        FVector End = Start + ArrowComponent->GetForwardVector() * TargetingMaxDistance;

        TArray<AActor*> ActorsToIgnore{ GetOwner() };

        TArray<FHitResult> HitResults;
        UKismetSystemLibrary::CapsuleTraceMultiForObjects(
            GetWorld(),
            Start, End,
            32.0f,
            TargetingMaxHeight,
            AllowedCollisionTypes, false, ActorsToIgnore, DebugMode, HitResults, true);


        for (const FHitResult& Hit : HitResults)
        {
            IIsTargetable* IsTargetable = Cast<IIsTargetable>(Hit.GetActor());

            if (Hit.bBlockingHit && IsTargetable != nullptr)
            {
                if (Hit.GetActor() != SelectedActor.Get())
                {
                    if (!IsAnythingBlockingTrace(Hit.GetActor(), ActorsToIgnore))
                    {
                        if (IsTargetable->IsTargetable())
                        {
                            Cast<IIsTargetable>(SelectedActor.Get())->OnDeselected();
                            SelectedActor = Hit.GetActor();

                            IsTargetable->OnSelected();

                            OnTargetChanged.Broadcast(SelectedActor.Get());
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }

    }

}


void UDynamicTargetingComponent::SetFreeCamera(bool bInFreeCamera)
{
    bIsFreeCamera = bInFreeCamera;
    UpdateIgnoreLookInput();
}

void UDynamicTargetingComponent::DisableCameraLock()
{
    if (bIsTargetingEnabled)
    {
        bIsTargetingEnabled = false;
        IIsTargetable* IsTargetable = Cast<IIsTargetable>(SelectedActor);

        if (IsTargetable != nullptr)
        {
            IsTargetable->OnDeselected();
            SelectedActor = nullptr;
            
            GetWorld()->GetTimerManager().ClearTimer(CheckTargetTimerHandle);
            UpdateIgnoreLookInput();

            OnTargetingToggled.Broadcast(false);            
        }
    }
}

FTransform UDynamicTargetingComponent::GetSelectedActorTransform() const
{
    if (SelectedActor.IsValid())
    {
        return SelectedActor->GetTransform();
    }
    else
    {
        return FTransform::Identity;
    }
}

bool UDynamicTargetingComponent::IsTargetingEnabled() const
{
    return bIsTargetingEnabled; 
}

void UDynamicTargetingComponent::UpdateTarget()
{
    if (!SelectedActor.IsValid())
    {
        return;
    }

    TArray<AActor*> ActorsToIgnore{ SelectedActor.Get() };

    if (IsAnythingBlockingTrace(SelectedActor.Get(), ActorsToIgnore))
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(DisableCameraLockTimerHandle))
        {
            if (DisableOnBlockDelay == 0.0f)
            {
                DisableCameraLock();
            }
            else
            {
                GetWorld()->GetTimerManager().SetTimer(
                    DisableCameraLockTimerHandle,
                    this,
                    &UDynamicTargetingComponent::DisableCameraLock,
                    DisableOnBlockDelay,
                    false);
            }
        }
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(DisableCameraLockTimerHandle);

        IIsTargetable* IsTargetable = Cast<IIsTargetable>(SelectedActor.Get());
        if (IsTargetable != nullptr)
        {
            if (!IsTargetable->IsTargetable())
            {
                DisableCameraLock();
                FindTarget();
            }
        }
    }
}


void UDynamicTargetingComponent::FindTarget()
{
    TArray<AActor*> OutActors;
    TArray<AActor*> PotentialTargets;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UIsTargetable::StaticClass(), OutActors);

    for (AActor* Actor : OutActors)
    {
        IIsTargetable* IsTargetable = Cast<IIsTargetable>(Actor);

        bool bIsTargetableActor = IsTargetable != nullptr;
        bool bIstargetable = IsTargetable->IsTargetable();
        bool bIsNotOwner = GetOwner() != Actor;

        if (bIsTargetableActor &&
            IsTargetable->IsTargetable() && 
            GetOwner() != SelectedActor &&
            bIsNotOwner)
        {
            FVector2D OutScreenPos;
            if (GetActorScreenPosition(Actor, OutScreenPos))
            {
                if (IsInViewport(OutScreenPos) && GetDistanceToOwner(Actor) <= TargetingMaxDistance)
                {
                    IsAnythingBlockingTrace(Actor, PotentialTargets);
                    PotentialTargets.Add(Actor);
                }
            }
        }
    }

    float DistanceFromCenterOfViewport = 0.0f;
    for (int i = 0; i < PotentialTargets.Num(); ++i)
    {
        AActor* Actor = PotentialTargets[i];

        FVector2D OutScreenPos;
        GetActorScreenPosition(Actor, OutScreenPos);
        float Val = FMath::Abs(OutScreenPos.X - UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X / 2);

        if (i == 0)
        {
            DistanceFromCenterOfViewport = Val;
            SelectedActor = Actor;
        }
        else
        {
            if (Val < DistanceFromCenterOfViewport)
            {
                DistanceFromCenterOfViewport = Val;
                SelectedActor = Actor;
            }
        }
    }

    if (SelectedActor.IsValid())
    {
        EnableCameraLock();
        OnTargetChanged.Broadcast(SelectedActor.Get());
    }
}

void UDynamicTargetingComponent::FindDirectionalTarget(bool bInOnLeft)
{
    TArray<AActor*> PotentialTargetsRight;
    TArray<AActor*> PotentialTargetsLeft;
    if (IsTargetingEnabled())
    {
        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UIsTargetable::StaticClass(), OutActors);

        for (AActor* Actor : OutActors)
        {
            IIsTargetable* IsTargetable = Cast<IIsTargetable>(Actor);
            if (IsTargetable != nullptr)
            {
                if (IsTargetable->IsTargetable() && 
                    Actor != SelectedActor && 
                    GetOwner() != SelectedActor &&
                    GetOwner() != Actor)
                {
                    if (GetDistanceToOwner(Actor) <= TargetingMaxDistance)
                    {
                        if (!IsAnythingBlockingTrace(Actor, PotentialTargetsRight))
                        {
                            if (!IsAnythingBlockingTrace(Actor, PotentialTargetsLeft))
                            {
                                if (IsTargetRightSide(Actor))
                                {
                                    PotentialTargetsRight.Add(Actor);
                                }
                                else
                                {
                                    PotentialTargetsLeft.Add(Actor);
                                }
                            }
                        }
                    }
                }
            }
        }

        AActor* LocalPotentialTarget;
        if (bInOnLeft)
        {
            if (HasArrayAnyElem(PotentialTargetsLeft))
            {
                LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetsLeft, true);
            }
            else
            {
                LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetsRight, false);
            }
        }
        else
        {
            if (HasArrayAnyElem(PotentialTargetsRight))
            {
                LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetsRight, true);
            }
            else
            {
                LocalPotentialTarget = GetTargetByDotProduct(PotentialTargetsLeft, false);
            }
        }

        if (GameUtils::IsValid(LocalPotentialTarget))
        {
            IIsTargetable* IsTargetable = Cast<IIsTargetable>(SelectedActor);
            if (IsTargetable != nullptr)
            {
                IsTargetable->OnDeselected();
                SelectedActor = LocalPotentialTarget;

                IsTargetable = Cast<IIsTargetable>(SelectedActor);
                if (IsTargetable != nullptr)
                {
                    IsTargetable->OnSelected();
                    OnTargetChanged.Broadcast(SelectedActor.Get());
                }
            }
        }
    }
}

void UDynamicTargetingComponent::EnableCameraLock()
{
    bIsTargetingEnabled = true;
    GetWorld()->GetTimerManager().ClearTimer(DisableCameraLockTimerHandle);
    SetDebugMode();

    IIsTargetable* IsTargetable = Cast<IIsTargetable>(SelectedActor);

    if (IsTargetable != nullptr)
    {
        IsTargetable->OnSelected();

        GetWorld()->GetTimerManager().SetTimer(
            CheckTargetTimerHandle, this, &UDynamicTargetingComponent::UpdateTarget, 0.016f, true);

        UpdateIgnoreLookInput();
        OnTargetingToggled.Broadcast(true);
    }
}

void UDynamicTargetingComponent::UpdateCameraLock()
{
    if (IsTargetingEnabled() && SelectedActor.IsValid())
    {
        float Distance = GetOwner()->GetDistanceTo(SelectedActor.Get());
        if (Distance >= 50.0f && Distance <= TargetingMaxDistance)
        {
            float DeltaTime = GetWorld()->GetDeltaSeconds();

            if (ArrowComponent.IsValid())
            {
                FRotator Current = ArrowComponent->GetComponentRotation();
                FRotator Target = GetOwner()->GetInstigator()->GetControlRotation();
                
                float Yaw = UKismetMathLibrary::RInterpTo_Constant(Current, Target, DeltaTime, 250.0f).Yaw;
                ArrowComponent->SetWorldRotation(FRotator(0, Yaw, 0));
            }

            if (!bIsFreeCamera)
            {
                FVector Start = GetOwner()->GetActorLocation();
                Start.Z += TraceHeightOffset;
                FVector End = SelectedActor->GetActorLocation();

                FRotator CurrentRot = GetOwner()->GetInstigator()->GetControlRotation();
                FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(Start, End);

                TargetRot.Pitch = GetOwner()->GetDistanceTo(SelectedActor.Get()) <= 300.0f ?
                    FMath::Clamp(TargetRot.Pitch, -25.0f, 25.0f) : TargetRot.Pitch;

                FRotator NewRot = UKismetMathLibrary::RInterpTo_Constant(CurrentRot, TargetRot, DeltaTime, 300.0f);

                GetOwner()->GetInstigator()->GetController()->SetControlRotation(NewRot);
            }
        }
        else
        {
            DisableCameraLock();
        }
    }
}

void UDynamicTargetingComponent::SetDebugMode()
{
    if (bDebug)
    {
        DebugMode = EDrawDebugTrace::Type::ForDuration;
    }
    else
    {
        DebugMode = EDrawDebugTrace::Type::None;
    }
}

void UDynamicTargetingComponent::UpdateIgnoreLookInput()
{
    AController* Controller = GetOwner()->GetInstigator()->GetController();

    if (GameUtils::IsValid(Controller))
    {
        if (IsTargetingEnabled() && !bIsFreeCamera)
        {
            Controller->SetIgnoreLookInput(true);
        }
        else
        {
            Controller->ResetIgnoreLookInput();
        }
    }
}

void UDynamicTargetingComponent::UpdateCollisionTypeArrays()
{
    for (TEnumAsByte<EObjectTypeQuery> ObjType : AllowedCollisionTypes)
    {
        BlockingCollisionTypes.Remove(ObjType);
    }
}

bool UDynamicTargetingComponent::IsInViewport(FVector2D InScreenPosition) const
{
    FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

    return InScreenPosition.X > 0 &&
        InScreenPosition.Y > 0 &&
        InScreenPosition.X <= ViewportSize.X &&
        InScreenPosition.Y <= ViewportSize.Y;
}

FVector UDynamicTargetingComponent::GetLineTraceStartLocation() const
{
    return GetOwner()->GetActorLocation() +
        GetOwner()->GetActorForwardVector() * TraceDepthOffset +
        GetOwner()->GetActorUpVector() * TraceHeightOffset;
}


AActor* UDynamicTargetingComponent::GetTargetByDotProduct(const TArray<AActor*>& InActors, bool bInBest) const
{
    float LocalDotProduct = 0.0f;
    AActor* LocalPotentialTarget = nullptr;

    for (int i = 0; i < InActors.Num(); ++i)
    {
        AActor* Actor = InActors[i];
        float DotProduct = CalculateDotProductToTarget(Actor);

        if (i == 0)
        {
            LocalDotProduct = DotProduct;
            LocalPotentialTarget = Actor;
        }
        else
        {
            bool bCondition = bInBest ? DotProduct > LocalDotProduct : DotProduct < LocalDotProduct;
            if (bCondition)
            {
                LocalDotProduct = DotProduct;
                LocalPotentialTarget = Actor;
            }
        }

    }

    return LocalPotentialTarget;
}

bool UDynamicTargetingComponent::IsAnythingBlockingTrace(AActor* InTarget, const TArray<AActor*>& InActorsToIgnore) const
{
    if (BlockingCollisionTypes.Num() > 0)
    {
        FVector Start = GetLineTraceStartLocation();
        FVector End = InTarget->GetActorLocation();

        FHitResult OutHit;

        if (UKismetSystemLibrary::LineTraceSingleForObjects(
            GetWorld(), Start, End, BlockingCollisionTypes, false, InActorsToIgnore, DebugMode, OutHit, true))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool UDynamicTargetingComponent::GetActorScreenPosition(AActor* InActor, FVector2D& OutScreenPos) const
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
    return UGameplayStatics::ProjectWorldToScreen(PlayerController, InActor->GetActorLocation(), OutScreenPos);
}

float UDynamicTargetingComponent::GetDistanceToOwner(AActor* InOtherActor) const
{
    return GetOwner()->GetDistanceTo(InOtherActor);
}

bool UDynamicTargetingComponent::IsTargetRightSide(AActor* InPotentialTarget) const
{
    if (!SelectedActor.IsValid())
    {
        return false;
    }

    FVector A, B;

    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = SelectedActor->GetActorLocation();
        A = UKismetMathLibrary::GetDirectionUnitVector(From, To);
    }

    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = InPotentialTarget->GetActorLocation();
        B = UKismetMathLibrary::GetDirectionUnitVector(From, To);
    }

    FVector Cross = UKismetMathLibrary::Cross_VectorVector(A, B);

    if (UKismetMathLibrary::Dot_VectorVector(Cross, GetOwner()->GetActorUpVector()) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float UDynamicTargetingComponent::CalculateDotProductToTarget(AActor* InTarget) const
{
    if (!SelectedActor.IsValid())
    {
        return 0.0f;
    }

    FVector A, B;
    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = SelectedActor->GetActorLocation();
        A = UKismetMathLibrary::GetDirectionUnitVector(From, To);
    }
    
    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = InTarget->GetActorLocation();
        B = UKismetMathLibrary::GetDirectionUnitVector(From, To);
    }

    return UKismetMathLibrary::Dot_VectorVector(A, B);
}

bool UDynamicTargetingComponent::HasArrayAnyElem(const TArray<AActor*>& Actors) const
{
    if (Actors.Num() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

