// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpaledArrowActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AImpaledArrowActor::AImpaledArrowActor()
    :LifeTime(15.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Scene");

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMeshComponent->SetRelativeLocation(FVector(-70.0f, 0.0f, 0.0f));
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->ComponentTags.Add("Dissolve");
    StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AImpaledArrowActor::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    StaticMeshComponent = nullptr;

    Super::EndPlay(EndPlayResult);
}

void AImpaledArrowActor::Init(UStaticMesh* const InMesh)
{
    StaticMeshComponent->SetStaticMesh(InMesh);
    SetLifeSpan(LifeTime);
    GetOwner()->OnDestroyed.AddDynamic(this, &AImpaledArrowActor::OnDestroyed);
}


void AImpaledArrowActor::OnDestroyed(AActor* DestroyedActor)
{
    Destroy();
}