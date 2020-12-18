// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellIndicatorActor.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ASpellIndicatorActor::ASpellIndicatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    FString AssetReference = TEXT("/Game/DynamicCombatSystem/VFX/Materials/MI_SpellIndicatror_01");
    ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialClass(*AssetReference);

    if (MaterialClass.Object == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find %s"), *AssetReference);
    }

    DecalMaterial = MaterialClass.Object;

    Radius = 256.0f;
}

// Called when the game starts or when spawned
void ASpellIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASpellIndicatorActor::OnConstruction(const FTransform& Transform)
{
    SetRadius(Radius);

    SetMaterial(DecalMaterial);
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