// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathActor.h"
#include "Components/BillboardComponent.h"
#include "Components/SplineComponent.h"
#include "GameCore/GameUtils.h"

// Sets default values
APatrolPathActor::APatrolPathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Scene");

    Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
    Billboard->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Billboard->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
    Billboard->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

    static UTexture2D* LoadedTexture = GameUtils::LoadAssetObject<UTexture2D>(
        "/Game/DynamicCombatSystem/Widgets/Textures/T_PatrolPath");

    Billboard->Sprite = LoadedTexture;

    PatrolSpline = CreateDefaultSubobject<USplineComponent>("PatrolSpline");
    PatrolSpline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    PatrolSpline->SetRelativeLocation(FVector(-50.0f, 0.0f, 10.0f));
    PatrolSpline->EditorSelectedSplineSegmentColor = FLinearColor(0.728f, 0.364f, 0.003f);
}

void APatrolPathActor::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    Billboard = nullptr;
    PatrolSpline = nullptr;
    Super::EndPlay(EndPlayResult);
}

void APatrolPathActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    PatrolSpline->SetClosedLoop(bCloseLoop);
}

