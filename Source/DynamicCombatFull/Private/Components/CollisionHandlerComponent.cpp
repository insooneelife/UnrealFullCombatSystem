// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionHandlerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UCollisionHandlerComponent::UCollisionHandlerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    ObjectTypesToCollideWith = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
    TraceRadius = 0.1f;
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

void UCollisionHandlerComponent::DeactivateCollision()
{
    bCollisionActive = false;
    bCanPerformTrace = false;
    OnCollisionDeactivated.Broadcast();
}

void UCollisionHandlerComponent::ActivateCollision(ECollisionPart InCollisionPart)
{
    HitActors.Empty();
    bCollisionActive = true;
    OnCollisionActivated.Broadcast(InCollisionPart);
}

void UCollisionHandlerComponent::SetCollisionMeshes(const TArray<FCollisionComponent>& InCollComps)
{
    CollisionComponents = InCollComps;
    UpdateLastSocketPositions();
}

void UCollisionHandlerComponent::SetCollisionMesh(UPrimitiveComponent* InWeaponMesh, const TArray<FName>& InSockets)
{
    FCollisionComponent CollComp;
    CollComp.Component = InWeaponMesh;
    CollComp.Sockets = InSockets;

    TArray<FCollisionComponent> CollComps = { CollComp };
    SetCollisionMeshes(CollComps);
}

void UCollisionHandlerComponent::AddIgnoredClass(TSubclassOf<AActor> IgnoredClass)
{
    IgnoredClasses.Add(IgnoredClass);
}

void UCollisionHandlerComponent::AddIgnoredCollisionProfileNames(FName InProfileName)
{
    IgnoredCollisionProfileNames.Add(InProfileName);
}

void UCollisionHandlerComponent::AddObjectTypesToCollideWith(TEnumAsByte<EObjectTypeQuery> InObjectType)
{
    ObjectTypesToCollideWith.Add(InObjectType);
}

void UCollisionHandlerComponent::UpdateLastSocketPositions()
{
    for (const FCollisionComponent& CollComp : CollisionComponents)
    {
        if (GameUtils::IsValid(CollComp.Component))
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
        if (GameUtils::IsValid(CollComp.Component))
        {
            for (FName Name : CollComp.Sockets)
            {
                FName UniqueName = GetUniqueSocketName(CollComp.Component, Name);
                FVector Start = LastSocketLocations[UniqueName];

                TArray<AActor*> IgnoreActors = GetHitActorsOrAddOwner(CollComp.Component);

                FVector End = CollComp.Component->GetSocketLocation(Name);

                TArray<FHitResult> OutHits;
                UKismetSystemLibrary::SphereTraceMultiForObjects(
                    GetWorld(), Start, End, TraceRadius, ObjectTypesToCollideWith, false,
                    IgnoreActors, VisualizeTrace, OutHits, true);

                for (const FHitResult& Hit : OutHits)
                {
                    if (Hit.GetActor() == GetOwner())
                        continue;

                    bool bActorNotContained = !GetHitActorsOrAddOwner(CollComp.Component).Contains(Hit.GetActor());
                    bool bNotIgnoredClass = !IsIgnoredClass(Hit.GetActor()->GetClass());
                    bool bCollisionProfileNotContained = 
                        !IgnoredCollisionProfileNames.Contains(Hit.Component->GetCollisionProfileName());

                    if (bActorNotContained &&
                        bNotIgnoredClass &&
                        bCollisionProfileNotContained)
                    {
                        AddHitActor(CollComp.Component, Hit.GetActor());
                        OnHit.Broadcast(Hit);
                    }
                }
            }
        }
    }
}

bool UCollisionHandlerComponent::IsIgnoredClass(TSubclassOf<AActor> InTestClass) const
{
    for (TSubclassOf<AActor> ClassType : IgnoredClasses)
    {
        if (UKismetMathLibrary::ClassIsChildOf(InTestClass, ClassType))
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

FName UCollisionHandlerComponent::GetUniqueSocketName(UPrimitiveComponent* InComponent, FName InSocketName) const
{
    return FName(*(UKismetSystemLibrary::GetDisplayName(InComponent) + InSocketName.ToString()));
}

int UCollisionHandlerComponent::GetHitActorsIndex(UPrimitiveComponent* InComponent) const
{
    for (int i = 0; i < HitActors.Num(); ++i)
    {
        const FCollCompHitActors& Data = HitActors[i];

        if (Data.Component == InComponent)
        {
            return i;
        }
    }
    return -1;
}

TArray<AActor*> UCollisionHandlerComponent::GetHitActorsOrAddOwner(UPrimitiveComponent* InComponent)
{
    int Index = GetHitActorsIndex(InComponent);
    if (Index >= 0)
    {
        return HitActors[Index].HitActors;
    }
    else
    {
        TArray<AActor*> Actors{ GetOwner() };

        FCollCompHitActors CollCompHitActors;
        CollCompHitActors.Component = InComponent;
        CollCompHitActors.HitActors = Actors;

        int AddIndex = HitActors.Add(CollCompHitActors);

        return HitActors[AddIndex].HitActors;
    }
}

void UCollisionHandlerComponent::AddHitActor(UPrimitiveComponent* InComponent, AActor* InHitActor)
{
    int Index = GetHitActorsIndex(InComponent);
    if (Index >= 0)
    {
        HitActors[Index].HitActors.Add(InHitActor);
    }
}