// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilityBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

#include "GamePlay/BaseCharacter.h"

void APlayerAbilityBase::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerAbilityBase::Init(UAbilityComponent* InAbilityComponent)
{
    Super::Init(InAbilityComponent);
    PlayerCharacter = Cast<ABaseCharacter>(Character);
}

FTransform APlayerAbilityBase::GetSurfaceTransform(float Range, float MaxSurfaceAngle, float MaxHeightDeviation) const
{
    FVector TraceStart = GetOwner()->GetActorLocation();
    FVector TraceEnd = TraceStart + UKismetMathLibrary::Conv_RotatorToVector(Character->GetControlRotation()) * Range;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = 
    {
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic)
    };

    TArray<AActor*> ActorsToIgnore;

    FHitResult OutHit;
    UKismetSystemLibrary::LineTraceSingleForObjects(
        GetWorld(), TraceStart, TraceEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::Type::None, OutHit, true);

    FVector HitLocation = OutHit.Location;

    OutHit.TraceEnd;

    if (OutHit.bBlockingHit)
    {
        float Dot = FVector::DotProduct(OutHit.Normal, FVector::UpVector);

        if (UKismetMathLibrary::DegAcos(Dot) <= FMath::Abs(MaxSurfaceAngle) && 
            IsInHeightRange(HitLocation, MaxHeightDeviation))
        {
            return FTransform(Character->GetControlRotation(), HitLocation);
        }
    }

    FVector HitDirection = UKismetMathLibrary::GetDirectionUnitVector(TraceStart, HitLocation);

    float TraceLength = (HitLocation - TraceStart).Size();
    const float SubtractedDistanceEachIteration = 5.0f;

    int Index = UKismetMathLibrary::FTrunc(TraceLength / SubtractedDistanceEachIteration);

    for (int i = 1; i <= Index; ++i)
    {
        FVector StartLocation = (TraceLength - (SubtractedDistanceEachIteration * i))* HitDirection + TraceStart;

        FVector OutGroundLocation;
        FVector OutNormal;
        if (FindGroundLocation(StartLocation, 0.0f, OutGroundLocation, OutNormal))
        {
            float Dot = FVector::DotProduct(OutHit.Normal, FVector::UpVector);

            if (UKismetMathLibrary::DegAcos(Dot) <= FMath::Abs(MaxSurfaceAngle) &&
                IsInHeightRange(HitLocation, MaxHeightDeviation))
            {
                return FTransform(Character->GetControlRotation(), OutGroundLocation);
            }
            
        }
    }

    return FTransform(Character->GetControlRotation(), GetOwner()->GetActorLocation());
}

FTransform APlayerAbilityBase::GetCrosshairTransform(FName SpawnSocket) const
{
    FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(SpawnSocket);
    FVector CrosshairDirection = PlayerCharacter->GetFollowCamera()->GetForwardVector();


    FVector ToCameraBoom =
        PlayerCharacter->GetCameraBoom()->GetComponentLocation() - 
        PlayerCharacter->GetFollowCamera()->GetComponentLocation();

    FVector TraceStart = 
        PlayerCharacter->GetFollowCamera()->GetComponentLocation() + CrosshairDirection* ToCameraBoom.Size();
    FVector TraceEnd = PlayerCharacter->GetFollowCamera()->GetComponentLocation() + CrosshairDirection * 10000.0f;

    // Projectile == ECC_GameTraceChannel1  
    // defined DefaultEngine.ini
    ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
    TArray<AActor*> ActorsToIgnore{ GetOwner() };

    FHitResult OutHit;
    UKismetSystemLibrary::CapsuleTraceSingle(
        GetWorld(),
        TraceStart, TraceEnd, 1.0f, 1.0f, TraceType, false, ActorsToIgnore, EDrawDebugTrace::Type::None, OutHit, true);

    FRotator SpawnDirection;
    if (OutHit.bBlockingHit)
    {
        SpawnDirection = UKismetMathLibrary::Conv_VectorToRotator(CrosshairDirection);
    }
    else
    {
        SpawnDirection = UKismetMathLibrary::Conv_VectorToRotator(SpawnLocation);
    }

    return FTransform(SpawnDirection, SpawnLocation);
}

FTransform APlayerAbilityBase::GetBeamTransform(
    float Range, float Radius, FName SpawnSocket, bool bGoesThroughObjects) const
{
    FVector StartTrace = Character->GetMesh()->GetSocketLocation(SpawnSocket);
    AActor* Controller = GetOwner()->GetInstigator()->GetController();
    FVector Forward = Controller->GetActorForwardVector();
    FVector EndTrace = Character->GetActorLocation() + (Forward * Range);

    if (bGoesThroughObjects)
    {
        return FTransform(Character->GetControlRotation(), EndTrace);
    }
    else
    {
        TArray<AActor*> ActorsToIgnore{ GetOwner() };
        FHitResult OutHit;
        ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
        UKismetSystemLibrary::SphereTraceSingle(
            GetWorld(), 
            StartTrace, EndTrace, Radius, TraceType, false, ActorsToIgnore, EDrawDebugTrace::Type::None, OutHit, true);

        if (OutHit.bBlockingHit)
        {
            const float ExtraLengthOnHit = 100.0f;
            FVector Location = OutHit.Location + Controller->GetActorForwardVector() * ExtraLengthOnHit;

            return FTransform(Character->GetControlRotation(), Location);
        }
        else
        {
            return FTransform(Character->GetControlRotation(), EndTrace);
        }
    }
}