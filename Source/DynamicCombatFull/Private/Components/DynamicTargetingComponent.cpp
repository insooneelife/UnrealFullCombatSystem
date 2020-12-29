// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicTargetingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Interfaces/IsTargetable.h"

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


// Called when the game starts
void UDynamicTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

    SetDebugMode();
    CheckCollisionTypeArrays();
    CharacterReference = Cast<ACharacter>(GetOwner());
    if (CharacterReference == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Casting to owner as character has failed!!"));
    }
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
    if (InArrowComp->IsValidLowLevel())
    {
        ArrowComponent = InArrowComp;
        ArrowComponent->SetUsingAbsoluteRotation(true);
    }
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

void UDynamicTargetingComponent::FindTargetWithAxisInput(float AxisValue)
{
    const float StartRotatingThreshold = 1.5f;
    if (ArrowComponent->IsValidLowLevel() &&
        SelectedActor->IsValidLowLevel() &&
        FMath::Abs(AxisValue) > StartRotatingThreshold &&
        !bIsFreeCamera)
    {
        FRotator A = GetOwner()->GetInstigator()->GetControlRotation();
        FRotator B = ArrowComponent->GetComponentRotation();
        FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(A, B);

        if (FMath::Abs(Delta.Yaw) <= 165.0f)
        {
            float Yaw = AxisValue * AxisInputSensitivity;
            float Val = UKismetSystemLibrary::MakeLiteralFloat(625.0f) * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
            float Min = -Val;
            float Max = Val;
            ArrowComponent->AddLocalRotation(FRotator(Min, Yaw, Max));
        }

        FVector Start = ArrowComponent->GetComponentLocation();
        FVector End = Start + ArrowComponent->GetForwardVector() * TargetingMaxDistance;

        TArray<AActor*> ActorsToIgnore;

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
                if (Hit.GetActor() != SelectedActor)
                {
                    if (!IsAnythingBlockingTrace(Hit.GetActor(), ActorsToIgnore))
                    {
                        if (IsTargetable->IsTargetable())
                        {
                            Cast<IIsTargetable>(SelectedActor)->OnDeselected();
                            SelectedActor = Hit.GetActor();
                            IsTargetable->OnSelected();

                            OnTargetChanged.Broadcast(SelectedActor);
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

bool UDynamicTargetingComponent::IsTargetingEnabled() const
{
    return bIsTargetingEnabled;
}

void UDynamicTargetingComponent::FindTargetOnLeft()
{
    return FindDirectionalTarget(true);
}

void UDynamicTargetingComponent::FindTargetOnRight()
{
    return FindDirectionalTarget(false);
}

void UDynamicTargetingComponent::SetFreeCamera(bool bFreeCamera)
{
    bIsFreeCamera = bFreeCamera;
    UpdateIgnoreLookInput();
}

void UDynamicTargetingComponent::FindTarget()
{
    TArray<AActor*> OutActors;
    TArray<AActor*> PotentialTargets;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UIsTargetable::StaticClass(), OutActors);

    for (AActor* Actor : OutActors)
    {
        IIsTargetable* IsTargetable = Cast<IIsTargetable>(Actor);

        if (IsTargetable != nullptr && 
            IsTargetable->IsTargetable() && 
            GetOwner() != SelectedActor)
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

    if (SelectedActor->IsValidLowLevel())
    {
        EnableCameraLock();
        OnTargetChanged.Broadcast(SelectedActor);
    }
}

bool UDynamicTargetingComponent::IsInViewport(FVector2D ScreenPosition)
{
    FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

    return ScreenPosition.X > 0 &&
        ScreenPosition.Y > 0 &&
        ScreenPosition.X <= ViewportSize.X &&
        ScreenPosition.Y <= ViewportSize.Y;
}

FVector UDynamicTargetingComponent::GetLineTraceStartLocation() const
{
    return GetOwner()->GetActorLocation() +
        GetOwner()->GetActorForwardVector() * TraceDepthOffset +
        GetOwner()->GetActorUpVector() * TraceHeightOffset;
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
            CheckTargetTimerHandle, this, &UDynamicTargetingComponent::CheckTarget, 0.016f, true);

        UpdateIgnoreLookInput();
        OnTargetingToggled.Broadcast(true);
    }
}

void UDynamicTargetingComponent::UpdateCameraLock()
{
    if (IsTargetingEnabled())
    {
        float Distance = GetOwner()->GetDistanceTo(SelectedActor);
        if (Distance >= 50.0f && Distance <= TargetingMaxDistance)
        {
            float DeltaTime = GetWorld()->GetDeltaSeconds();
            if (ArrowComponent->IsValidLowLevel())
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

                TargetRot.Pitch = GetOwner()->GetDistanceTo(SelectedActor) <= 300.0f ?
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

void UDynamicTargetingComponent::CheckTarget()
{
    TArray<AActor*> ActorsToIgnore{ SelectedActor };

    if (IsAnythingBlockingTrace(SelectedActor, ActorsToIgnore))
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

        IIsTargetable* IsTargetable = Cast<IIsTargetable>(SelectedActor);
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

void UDynamicTargetingComponent::FindDirectionalTarget(bool bOnLeft)
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
                if (IsTargetable->IsTargetable() && Actor != SelectedActor && GetOwner() != SelectedActor)
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
        if (bOnLeft)
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

        if (LocalPotentialTarget->IsValidLowLevel())
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
                    OnTargetChanged.Broadcast(SelectedActor);
                }
            }
        }
    }
}

AActor* UDynamicTargetingComponent::GetTargetByDotProduct(const TArray<AActor*> Actors, bool bBest) const
{
    float LocalDotProduct = 0.0f;
    AActor* LocalPotentialTarget = nullptr;

    for (int i = 0; i < Actors.Num(); ++i)
    {
        AActor* Actor = Actors[i];

        float DotProduct = CalculateDotProductToTarget(Actor);

        

        if (i == 0)
        {
            LocalDotProduct = DotProduct;
            LocalPotentialTarget = Actor;
        }
        else
        {
            bool bCondition = bBest ? DotProduct > LocalDotProduct : DotProduct < LocalDotProduct;
            if (bCondition)
            {
                LocalDotProduct = DotProduct;
                LocalPotentialTarget = Actor;
            }
        }

    }

    return LocalPotentialTarget;
}

void UDynamicTargetingComponent::UpdateIgnoreLookInput()
{
    AController* Controller = GetOwner()->GetInstigator()->GetController();
    if (Controller->IsValidLowLevel())
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

bool UDynamicTargetingComponent::IsAnythingBlockingTrace(AActor* Target, const TArray<AActor*>& ActorsToIgnore) const
{
    if (BlockingCollisionTypes.Num() > 0)
    {
        FVector Start = GetLineTraceStartLocation();
        FVector End = Target->GetActorLocation();

        FHitResult OutHit;

        if (UKismetSystemLibrary::LineTraceSingleForObjects(
            GetWorld(), Start, End, BlockingCollisionTypes, false, ActorsToIgnore, DebugMode, OutHit, true))
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

void UDynamicTargetingComponent::CheckCollisionTypeArrays()
{
    for (TEnumAsByte<EObjectTypeQuery> ObjType : AllowedCollisionTypes)
    {
        BlockingCollisionTypes.Remove(ObjType);
    }
}

bool UDynamicTargetingComponent::GetActorScreenPosition(AActor* InActor, FVector2D& OutScreenPos) const
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
    return UGameplayStatics::ProjectWorldToScreen(PlayerController, InActor->GetActorLocation(), OutScreenPos);
}

float UDynamicTargetingComponent::GetDistanceToOwner(AActor* OtherActor) const
{
    return GetOwner()->GetDistanceTo(OtherActor);
}

bool UDynamicTargetingComponent::IsTargetRightSide(AActor* PotentialTarget) const
{
    FVector A, B;

    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = SelectedActor->GetActorLocation();
        A = UKismetMathLibrary::GetDirectionUnitVector(From, To);
    }

    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = PotentialTarget->GetActorLocation();
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

float UDynamicTargetingComponent::CalculateDotProductToTarget(AActor* Target) const
{
    FVector A, B;
    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = SelectedActor->GetActorLocation();
        A = UKismetMathLibrary::GetDirectionUnitVector(From, To);
    }
    
    {
        FVector From = GetOwner()->GetActorLocation();
        FVector To = Target->GetActorLocation();
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
