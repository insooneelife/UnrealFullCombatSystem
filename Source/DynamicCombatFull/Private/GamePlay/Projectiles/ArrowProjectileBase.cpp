// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CollisionHandlerComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/EffectsComponent.h"
#include "Components/BehaviorComponent.h"
#include "GamePlay/Items/ObjectItems/ArrowItem.h"
#include "GameCore/DefaultGameInstance.h"
#include "Interfaces/CanBeAttacked.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/ImpaledArrowActor.h"

// Sets default values
AArrowProjectileBase::AArrowProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    HeadShotDamageMultiplier = 2.0f;
    InitialSpeed = 7000.0f;
    LifeTime = 15.0f;
    ImpulsePower = 20000.0f;

    TSubclassOf<AImpaledArrowActor> LoadedClass = GameUtils::LoadAssetClass<AImpaledArrowActor>(
        "/Game/DynamicCombatSystem/Blueprints/Projectiles/ImpaledArrowBP");
    ImpaledArrowClass = LoadedClass;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    RootComponent = StaticMesh;
    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
    ParticleSystem->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    CollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>("CollisionHandler");

    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    CollisionHandler->AddIgnoredCollisionProfileNames("Pawn");
    CollisionHandler->SetTraceRadius(0.5f);

    CollisionHandler->AddObjectTypesToCollideWith(
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

    CollisionHandler->AddObjectTypesToCollideWith(
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

    // interactable
    CollisionHandler->AddObjectTypesToCollideWith(
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
}

// Called when the game starts or when spawned
void AArrowProjectileBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AArrowProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CollisionHandler->OnHit.RemoveDynamic(this, &AArrowProjectileBase::OnHit);

    Super::EndPlay(EndPlayReason);
}

void AArrowProjectileBase::BeginPlayDelayed()
{
    ProjectileMovement->ProjectileGravityScale = 0.3f;
}

void AArrowProjectileBase::OnHit(const FHitResult& InHit)
{
    OnArrowHit(InHit);
}

void AArrowProjectileBase::OnArrowHit(const FHitResult& InHit)
{
    FVector HitLocation = InHit.Location;
    FVector HitNormal = InHit.Normal;
    AActor* HitActor = InHit.GetActor();
    UPrimitiveComponent* HitComponent = InHit.GetComponent();
    FName HitBoneName = InHit.BoneName;

    if (GetOwner() != HitActor)
    {
        ICanBeAttacked* CanBeAttacked = Cast<ICanBeAttacked>(HitActor);
        if (CanBeAttacked != nullptr)
        {
            if (IsEnemy(HitActor))
            {
                FName SocketName = CanBeAttacked->GetHeadSocket();

                EAttackResult AttackResult;
                FHitData HitData;

                HitData.Damage = SocketName == HitBoneName ? HeadShotDamageMultiplier * Damage : Damage;
                HitData.DamageCauser = GetOwner();
                HitData.HitFromDirection =
                    UKismetMathLibrary::GetDirectionUnitVector(HitLocation, HitActor->GetActorLocation());
                HitData.bCanReceiveImpact = false;
                HitData.bCanBeBlocked = true;
                HitData.bCanBeParried = false;

                bool bAttackResult = CanBeAttacked->TakeDamage(HitData, AttackResult);

                if (AttackResult != EAttackResult::Failed)
                {
                    if (bAttackResult)
                    {
                        UDefaultGameInstance* DefaultGameInstance = Cast<UDefaultGameInstance>(GetGameInstance());
                        DefaultGameInstance->PlayHitSound(GetOwner(), HitActor, HitLocation);

                        UEffectsComponent* EffectsComponent =
                            Cast<UEffectsComponent>(HitActor->GetComponentByClass(UEffectsComponent::StaticClass()));

                        if (GameUtils::IsValid(EffectsComponent))
                        {
                            EffectsComponent->ApplyEffect(
                                EEffectType::Stun, 1.0f, EApplyEffectMethod::Replace, GetOwner());
                        }
                    }

                    SpawnImpaledArrow(HitComponent, HitBoneName, HitActor, HitLocation);
                    ApplyHitImpulse(HitComponent, HitNormal, HitBoneName);
                    Destroy();
                }
            }

        }
        else
        {
            SpawnImpaledArrow(HitComponent, HitBoneName, HitActor, HitLocation);
            ApplyHitImpulse(HitComponent, HitNormal, HitBoneName);
            Destroy();
        }
    }
}

void AArrowProjectileBase::Init(float InDamage, float InInitialSpeed)
{
    Damage = InDamage;
    InitialSpeed = InInitialSpeed;
    ProjectileMovement->Velocity = GetActorForwardVector() * InitialSpeed;

    UpdateArrowMesh();
    SetLifeSpan(LifeTime);
    CollisionHandler->SetCollisionMesh(StaticMesh, StaticMesh->GetAllSocketNames());
    CollisionHandler->ActivateCollision(ECollisionPart::None);
    CollisionHandler->OnHit.AddDynamic(this, &AArrowProjectileBase::OnHit);

    FTimerHandle Unused;
    GetWorld()->GetTimerManager().SetTimer(Unused, this, &AArrowProjectileBase::BeginPlayDelayed, 0.3f, false);
}

void AArrowProjectileBase::ApplyHitImpulse(UPrimitiveComponent* InComponent, FVector InHitNormal, FName InBoneName)
{
    if (InComponent->IsAnySimulatingPhysics())
    {
        FVector Impulse = InHitNormal * -1.0f * ImpulsePower;
        InComponent->AddImpulse(Impulse, InBoneName);
    }
}

void AArrowProjectileBase::UpdateArrowMesh()
{
    UEquipmentComponent* EquipmentComponent = 
        Cast<UEquipmentComponent>(GetOwner()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (GameUtils::IsValid(EquipmentComponent))
    {
        TSubclassOf<UItemBase> ItemClass = EquipmentComponent->GetActiveItem(EItemType::Arrows, 0).ItemClass;
        UArrowItem* ArrowItem = NewObject<UArrowItem>(GetOwner(), ItemClass);

        StaticMesh->SetStaticMesh(ArrowItem->GetArrowMesh());
    }
}

bool AArrowProjectileBase::IsEnemy(AActor* InTarget) const
{
    UBehaviorComponent* BehaviorComponent = 
        Cast<UBehaviorComponent>(GetOwner()->GetComponentByClass(UBehaviorComponent::StaticClass()));

    // #fix BaseCharacter doesn't have BehaviorComponent 
    if (BehaviorComponent != nullptr)
    {
        return BehaviorComponent->IsEnemy(InTarget);
    }

    return true;
}

void AArrowProjectileBase::SpawnImpaledArrow(
    USceneComponent* InComponent,
    FName InSocketName, 
    AActor* InActor, 
    FVector InLocation)
{
    ProjectileMovement->StopMovementImmediately();

    FActorSpawnParameters Params;
    Params.Owner = InActor;
    Params.Instigator = GetOwner()->GetInstigator();
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FTransform SpawnTransform = FTransform(GetActorRotation(), InLocation, GetActorScale3D());

    AImpaledArrowActor* SpawnedActor = GetWorld()->SpawnActor<AImpaledArrowActor>(
        ImpaledArrowClass, SpawnTransform, Params);
    SpawnedActor->Init(StaticMesh->GetStaticMesh()); 
    SpawnedActor->AttachToComponent(InComponent, FAttachmentTransformRules::KeepWorldTransform, InSocketName);
}

