// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionHandlerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UCollisionHandlerComponent::UCollisionHandlerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    ObjectTypesToCollideWith = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
    TraceRadius = 0.1f;
}


// Called when the game starts
void UCollisionHandlerComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}


// Called every frame
void UCollisionHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bCollisionActive)
    {
        if (bCanPerformTrace)
        {
            PerformTrace();
        }

        UpdateLastSocketPositions();
        bCanPerformTrace = true;
    }
}

void UCollisionHandlerComponent::SetCollisionMeshes(const TArray<FCollisionComponent>& CollComps)
{
    CollisionComponents = CollComps;
    UpdateLastSocketPositions();
}

void UCollisionHandlerComponent::SetCollisionMesh(UPrimitiveComponent* WeaponMesh, const TArray<FName>& Sockets)
{
    FCollisionComponent CollComp;
    CollComp.Component = WeaponMesh;
    CollComp.Sockets = Sockets;

    TArray<FCollisionComponent> CollComps = { CollComp };
    SetCollisionMeshes(CollComps);
}

void UCollisionHandlerComponent::DeactivateCollision()
{
    bCollisionActive = false;
    bCanPerformTrace = false;
    OnCollisionDeactivated.Broadcast();
}

void UCollisionHandlerComponent::ActivateCollision(ECollisionPart CollisionPart)
{
    HitActors.Empty();
    bCollisionActive = true;
    OnCollisionActivated.Broadcast(CollisionPart);
}

void UCollisionHandlerComponent::UpdateLastSocketPositions()
{
    for (const FCollisionComponent& CollComp : CollisionComponents)
    {
        if (CollComp.Component->IsValidLowLevel())
        {
            for (FName Name : CollComp.Sockets)
            {
                FName SocketName = GetUniqueSocketName(CollComp.Component, Name);
                FVector SocketLoc = CollComp.Component->GetSocketLocation(Name);

                LastSocketLocations.Add(SocketName, SocketLoc);
            }
        }
    }
}

void UCollisionHandlerComponent::PerformTrace()
{
    for (const FCollisionComponent& CollComp : CollisionComponents)
    {
        if (CollComp.Component->IsValidLowLevel())
        {
            for (FName Name : CollComp.Sockets)
            {
                FName UniqueName = GetUniqueSocketName(CollComp.Component, Name);

                FVector Start = LastSocketLocations[UniqueName];

                TArray<AActor*> IgnoreActors = GetHitActors(CollComp.Component);
                FVector End = CollComp.Component->GetSocketLocation(Name);

                TArray<FHitResult> OutHits;
                UKismetSystemLibrary::SphereTraceMultiForObjects(
                    GetWorld(), Start, End, TraceRadius, ObjectTypesToCollideWith, false,
                    IgnoreActors, VisualizeTrace, OutHits, true);

                for (const FHitResult& Hit : OutHits)
                {
                    if (!GetHitActors(CollComp.Component).Contains(Hit.GetActor()) &&
                        !IsIgnoredClass(Hit.GetActor()->GetClass()) &&
                        !IgnoredCollisionProfileNames.Contains(Hit.Component->GetCollisionProfileName()))
                    {
                        AddHitActor(CollComp.Component, Hit.GetActor());

                        OnHit.Broadcast(Hit);
                    }
                }
            }
        }
    }
}

bool UCollisionHandlerComponent::IsIgnoredClass(TSubclassOf<AActor> TestClass) const
{
    for (TSubclassOf<AActor> ClassType : IgnoredClasses)
    {
        if (UKismetMathLibrary::ClassIsChildOf(TestClass, ClassType))
        {
            return true;
        }
    }
    return false;
}

bool UCollisionHandlerComponent::IsCollisionActive() const
{
    return bCanPerformTrace;
}

FName UCollisionHandlerComponent::GetUniqueSocketName(UPrimitiveComponent* Component, FName SocketName) const
{
    return FName(*(UKismetSystemLibrary::GetDisplayName(Component) + SocketName.ToString()));
}

int UCollisionHandlerComponent::GetHitActorsIndex(UPrimitiveComponent* Component) const
{
    for (int i = 0; i < HitActors.Num(); ++i)
    {
        const FCollCompHitActors& Data = HitActors[i];

        if (Data.Component == Component)
        {
            return i;
        }
    }
    return -1;
}

TArray<AActor*> UCollisionHandlerComponent::GetHitActors(UPrimitiveComponent* Component)
{
    int Index = GetHitActorsIndex(Component);
    if (Index >= 0)
    {
        return HitActors[Index].HitActors;
    }
    else
    {
        TArray<AActor*> Actors{ GetOwner() };

        FCollCompHitActors CollCompHitActors;
        CollCompHitActors.Component = Component;
        CollCompHitActors.HitActors = Actors;

        int AddIndex = HitActors.Add(CollCompHitActors);

        return HitActors[AddIndex].HitActors;

    }
}

void UCollisionHandlerComponent::AddHitActor(UPrimitiveComponent* Component, AActor* HitActor)
{
    int Index = GetHitActorsIndex(Component);
    if (Index >= 0)
    {
        HitActors[Index].HitActors.Add(HitActor);
    }
}