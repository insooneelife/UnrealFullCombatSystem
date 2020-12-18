// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPathActor.h"
#include "Components/BillboardComponent.h"

// Sets default values
APatrolPathActor::APatrolPathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolPathActor::BeginPlay()
{
	Super::BeginPlay();
	
    PatrolSpline->SetClosedLoop(bCloseLoop);

}

// Called every frame
void APatrolPathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

