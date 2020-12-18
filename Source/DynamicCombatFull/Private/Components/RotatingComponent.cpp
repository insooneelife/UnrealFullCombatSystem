// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/RotatingInterface.h"


URotatingComponent::URotatingComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    MaxDegreesPerSecond = 720.0f;
    MaxAllowedDegrees = 180.0f;
}

// Called when the game starts
void URotatingComponent::BeginPlay()
{
    Super::BeginPlay();

    IRotatingInterface* RotatingInterface = Cast<IRotatingInterface>(GetOwner());
    if (RotatingInterface == nullptr)
    {
        FString Name = UGameplayStatics::GetObjectClass(GetOwner())->GetDisplayNameText().ToString();
        UE_LOG(LogTemp, Warning, TEXT("Does not implement interface IRotatingInterface!  %s"), *Name);
    }
}


// Called every frame
void URotatingComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //Debug();
    if (bShouldRotate)
    {
        //float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
        TimeElapsed = TimeElapsed + DeltaTime;
        if (TimeElapsed <= RotateTime)
        {
            IRotatingInterface* RotatingInterface = Cast<IRotatingInterface>(GetOwner());
            if (RotatingInterface == nullptr)
            {
                FRotator CurrentRot = GetOwner()->GetActorRotation();
                FRotator TargetRot = RotatingInterface->GetDesiredRotation();

                FRotator NewRot = 
                    UKismetMathLibrary::RInterpTo_Constant(CurrentRot, TargetRot, DeltaTime, MaxDegreesPerSecond);

                GetOwner()->SetActorRotation(NewRot);
            }
        }
        else
        {
            StopRotating();
        }
    }
}

void URotatingComponent::StartRotatingWithLimit(float InMaxPossibleRotation, float InMaxDegreesPerSecond)
{
    RotateTime = (InMaxPossibleRotation / InMaxDegreesPerSecond);

    MaxDegreesPerSecond = InMaxDegreesPerSecond;
    TimeElapsed = 0.0f;
    bShouldRotate = true;
    OnRotatingStart.Broadcast();
}

void URotatingComponent::StartRotatingWithTime(float InTime, float InMaxDegreesPerSecond)
{
    MaxDegreesPerSecond = InMaxDegreesPerSecond;

    RotateTime = InTime;
    TimeElapsed = 0.0f;
    bShouldRotate = true;
    OnRotatingStart.Broadcast();
}

void URotatingComponent::StopRotating()
{
    bShouldRotate = false;
    OnRotatingEnd.Broadcast();
}

void URotatingComponent::Debug()
{
    IRotatingInterface* RotatingInterface = Cast<IRotatingInterface>(GetOwner());
    if (RotatingInterface == nullptr)
    {
        FRotator Rot = RotatingInterface->GetDesiredRotation();
        FVector Start = GetOwner()->GetActorLocation();
        FVector End = Start + UKismetMathLibrary::Conv_RotatorToVector(Rot) * 250.0f;

        UKismetSystemLibrary::DrawDebugArrow(GetWorld(), Start, End, 100.0f, FLinearColor::Red, 0.0f, 5.0f);
    }
}

void URotatingComponent::SetRotationMode(ERotationMode InMode)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (Character != nullptr)
    {
        if (InMode == ERotationMode::FaceCamera)
        {
            Character->bUseControllerRotationYaw = false;
            Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
            Character->GetCharacterMovement()->bOrientRotationToMovement = false;
        }
        else if(InMode == ERotationMode::OrientToMovement)
        {
            Character->bUseControllerRotationYaw = false;
            Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
            Character->GetCharacterMovement()->bOrientRotationToMovement = true;
        }
    }

}

bool URotatingComponent::IsRotating() const
{
    return bShouldRotate;
}





