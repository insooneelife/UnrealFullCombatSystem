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

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    RootComponent = StaticMesh;
    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
    ParticleSystem->AttachToComponent(StaticMesh, FAttachmentTransformRules::KeepRelativeTransform);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    CollisionHandler = CreateDefaultSubobject<UCollisionHandlerComponent>("CollisionHandler");
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
    FVector HitLocation = InHit.Location;
    FVector HitNormal = InHit.Normal;
    AActor* HitActor = InHit.GetActor();
    UPrimitiveComponent* HitComponent = InHit.GetComponent();
    FName HitBoneName = InHit.BoneName;

    GameUtils::PrintHitResult(InHit);

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
                HitData.bCanBeBlocked = true;

                bool bAttackResult = CanBeAttacked->TakeDamage(HitData, AttackResult);

                if (AttackResult != EAttackResult::Failed)
                {
                    if (bAttackResult)
                    {
                        UDefaultGameInstance* DefaultGameInstance = Cast<UDefaultGameInstance>(GetGameInstance());
                        DefaultGameInstance->PlayHitSound(GetOwner(), HitActor, HitLocation);

                        UEffectsComponent* EffectsComponent =
                            Cast<UEffectsComponent>(HitActor->GetComponentByClass(UEffectsComponent::StaticClass()));

                        if (EffectsComponent->IsValidLowLevel())
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

    if (EquipmentComponent->IsValidLowLevel())
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

    if (BehaviorComponent->IsValidLowLevel())
    {
        return BehaviorComponent->IsEnemy(InTarget);
    }

    return true;
}

