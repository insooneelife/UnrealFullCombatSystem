// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellIndicatorActor.h"
#include "Components/SceneComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameCore/GameUtils.h"

// Sets default values
ASpellIndicatorActor::ASpellIndicatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Scene = CreateDefaultSubobject<USceneComponent>("Scene");
    Decal = CreateDefaultSubobject<UDecalComponent>("Decal");
    Decal->SetupAttachment(Scene);

    static UMaterialInterface* LoadedObject = 
        GameUtils::LoadAssetObject<UMaterialInterface>(
            TEXT("/Game/DynamicCombatSystem/VFX/Materials/MI_SpellIndicatror_01"));

    SetMaterial(LoadedObject);
    Radius = 256.0f;
}

// Called when the game starts or when spawned
void ASpellIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASpellIndicatorActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    SetRadius(Radius);
}

void ASpellIndicatorActor::SetRadius(float NewRadius)
{
    float Val = NewRadius / Decal->DecalSize.Y;

    FVector NewScale = FVector(1.0f, Val, Val);

    Decal->SetWorldScale3D(NewScale);
}

void ASpellIndicatorActor::SetMaterial(UMaterialInterface* NewMaterial)
{
    Decal->SetDecalMaterial(NewMaterial);
}

void ASpellIndicatorActor::Show()
{
    SetActorHiddenInGame(false);
}

void ASpellIndicatorActor::Hide()
{
    SetActorHiddenInGame(true);
}