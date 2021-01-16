// Fill out your copyright notice in the Description page of Project Settings.


#include "SummonedMinionAICharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/DissolveComponent.h"
#include "Components/EquipmentComponent.h"
#include "GamePlay/AI/BaseAIController.h"
#include "GameCore/GameUtils.h"


ASummonedMinionAICharacter::ASummonedMinionAICharacter()
    :
    Duration(30.0f),
    CollisionSockets({
        "collision_socket_01", 
        "collision_socket_02", 
        "collision_socket_03", 
        "collision_socket_04", 
        "collision_socket_05"
    })
{
}

void ASummonedMinionAICharacter::BeginPlay()
{
    Super::BeginPlay();

    //MeleeCollisionHandler->OnCollisionActivated.RemoveDynamic(this, &ASummonedMinionAICharacter::OnCollisionActivated);
    //MeleeCollisionHandler->OnCollisionActivated.AddDynamic(this, &ASummonedMinionAICharacter::OnCollisionActivatedOverride);

    Equipment->SetCombat(true);
    DissolveInCharacterMesh();

    FTimerHandle DelayTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        DelayTimerHandle, this, &ASummonedMinionAICharacter::Delayed_BeginPlay, 0.0f, false);

    if (Duration > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            DeathTimerHandle, this, &AAICharacter::Death, Duration, false);
    }
}

void ASummonedMinionAICharacter::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    Super::EndPlay(EndPlayResult);
}

void ASummonedMinionAICharacter::OnOwnerStateChanged(EState InPrevState, EState InNewState)
{
    if (InNewState == EState::Dead)
    {
        if (StateManager->GetState() != EState::Dead)
        {
            Death();
            GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
        }
    }
}

bool ASummonedMinionAICharacter::CanEffectBeApplied(EEffectType Type, AActor* Applier)
{
    if (Type == EEffectType::Stun)
    {
        return Super::CanEffectBeApplied(Type, Applier);
    }
    else if (Type == EEffectType::Burning)
    {
        return Super::CanEffectBeApplied(Type, Applier);
    }

    return false;
}

void ASummonedMinionAICharacter::OnCollisionActivated(ECollisionPart CollisionPart)
{
    MeleeCollisionHandler->SetCollisionMesh(GetMesh(), CollisionSockets);
}

void ASummonedMinionAICharacter::DissolveInCharacterMesh()
{
    SetActorHiddenInGame(true);
    GetCharacterMovement()->DisableMovement();
    AIController->BrainComponent->StopLogic("DissolveInCharacterMesh");
    
    // #not working
    // Dissolve->StartDissolve(GetMesh(), true);

    FTimerHandle DelayTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        DelayTimerHandle, this, &ASummonedMinionAICharacter::Delayed_OnCollisionActivated, 0.1f, false);
}


void ASummonedMinionAICharacter::Delayed_OnCollisionActivated()
{
    SetActorHiddenInGame(false);

    FTimerHandle DelayTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        DelayTimerHandle, this, &ASummonedMinionAICharacter::Delayed_Delayed_OnCollisionActivated, 1.5f, false);
}

void ASummonedMinionAICharacter::Delayed_Delayed_OnCollisionActivated()
{
    if (StateManager->GetState() != EState::Dead)
    {
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        AIController->BrainComponent->RestartLogic();
    }
}

void ASummonedMinionAICharacter::Delayed_BeginPlay()
{
    if (GameUtils::IsValid(GetOwner()))
    {
        UStateManagerComponent* StateManagerComponent =
            Cast<UStateManagerComponent>(GetOwner()->GetComponentByClass(UStateManagerComponent::StaticClass()));

        if (GameUtils::IsValid(StateManagerComponent))
        {
            StateManagerComponent->OnStateChanged.AddDynamic(
                this, &ASummonedMinionAICharacter::OnOwnerStateChanged);
        }
    }
}