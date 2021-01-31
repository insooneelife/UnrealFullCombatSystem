// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTrapAbilityEffect.h"
#include "Sound/SoundBase.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StateManagerComponent.h"
#include "GameCore/GameUtils.h"

// Sets default values
ADamageTrapAbilityEffect::ADamageTrapAbilityEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    
    Damage = 15.0f;
    Impulse = 40000.0f;
    Duration = 60.0f;
    ActivationDelay = 1.0f;
    Color = FLinearColor(10.0f, 1.0f, 0.0f, 1.0f);

    static UParticleSystem* LoadedParticleObject = 
        GameUtils::LoadAssetObject<UParticleSystem>(TEXT("/Game/DynamicCombatSystem/VFX/P_Explosion"));
    HitParticle = LoadedParticleObject;

    static USoundBase* LoadedSoundObject = 
        GameUtils::LoadAssetObject<USoundBase>(TEXT("/Game/DynamicCombatSystem/SFX/CUE/CUE_GroundExplosion"));
    ExplosionSound = LoadedSoundObject;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
    SphereComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
    SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    DecalComponent = CreateDefaultSubobject<UDecalComponent>("Decal");
    DecalComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ADamageTrapAbilityEffect::BeginPlay()
{
    Super::BeginPlay();
}

void ADamageTrapAbilityEffect::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    this->SphereComponent->OnComponentBeginOverlap.RemoveDynamic(
        this, &ADamageTrapAbilityEffect::OnSphereBeginOverlap);

    SphereComponent = nullptr;
    DecalComponent = nullptr;
    HitParticle = nullptr;
    ExplosionSound = nullptr;
}

void ADamageTrapAbilityEffect::GetTraceObjects(TArray<FHitResult>& OutHits) const
{
    FVector Start = GetActorLocation();
    FVector End = GetActorLocation() + FVector(0, 0, 1);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes =
    {
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
    };

    TArray<AActor*> ActorsToIgnore{ GetOwner() };
    UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        Start, End, DamageRadius, ObjectTypes, false, ActorsToIgnore, DebugType, OutHits, true);
}

void ADamageTrapAbilityEffect::Init(
    float InDamageRadius,
    float InDamage,
    float InImpulse,
    float InTrapRadius,
    float InDuration,
    float InActivationDelay)
{
    Super::Init(InDamageRadius, InDamage, InImpulse, false);
    TrapRadius = InTrapRadius;
    Duration = InDuration;
    ActivationDelay = InActivationDelay;

    this->SphereComponent->OnComponentBeginOverlap.AddDynamic(
        this, &ADamageTrapAbilityEffect::OnSphereBeginOverlap);

    SetLifeSpan(Duration);
    SphereComponent->SetSphereRadius(TrapRadius);
}

void ADamageTrapAbilityEffect::OnSphereBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (GetOwner() != OtherActor && IsEnemy(OtherActor))
    {
        if(!IsImmortal(OtherActor))
        {
            ExecuteTrap();
        }
        else
        {
            UStateManagerComponent* StateManager = Cast<UStateManagerComponent>(
                OtherActor->GetComponentByClass(UStateManagerComponent::StaticClass()));

            if (GameUtils::IsValid(StateManager))
            {
                StateManager->OnActivityChanged.AddDynamic(
                    this, &ADamageTrapAbilityEffect::OnTargetActivityChanged);
            }
        }
    }
}

void ADamageTrapAbilityEffect::OnTargetActivityChanged(EActivity InActivity, bool bInValue)
{
    if (InActivity == EActivity::IsImmortal && !bInValue)
    {
        ExecuteTrap();
    }
}

void ADamageTrapAbilityEffect::EnableTrap()
{
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

void ADamageTrapAbilityEffect::DisableTrap()
{
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ADamageTrapAbilityEffect::SpawnHitParticle()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation());
}

void ADamageTrapAbilityEffect::UpdateDecal()
{
    float Val = TrapRadius / DecalComponent->DecalSize.Y;
    FVector Scale(1.0f, Val, Val);
    DecalComponent->SetWorldScale3D(Scale);

    UMaterialInstanceDynamic* MaterialInstance = DecalComponent->CreateDynamicMaterialInstance();

    MaterialInstance->SetVectorParameterValue(TEXT("Color"), Color);
}

void ADamageTrapAbilityEffect::PlaySound()
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

bool ADamageTrapAbilityEffect::IsImmortal(AActor* InActor) const
{
    UStateManagerComponent* StateManager =
        Cast<UStateManagerComponent>(InActor->GetComponentByClass(UStateManagerComponent::StaticClass()));

    if (GameUtils::IsValid(StateManager))
    {
        if (StateManager->GetActivityValue(EActivity::IsImmortal))
        {
            return true;
        }
    }

    return false;
}

void ADamageTrapAbilityEffect::ExecuteTrap()
{
    DisableTrap();
    DamageToShapeArea();
    SpawnHitParticle();
    PlaySound();
    Destroy();
}