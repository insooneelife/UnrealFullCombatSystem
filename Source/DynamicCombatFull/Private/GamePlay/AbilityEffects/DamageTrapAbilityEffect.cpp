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

    RootComponent = CreateDefaultSubobject<USceneComponent>("Scene");
    SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
    DecalComponent = CreateDefaultSubobject<UDecalComponent>("Decal");
}

// Called when the game starts or when spawned
void ADamageTrapAbilityEffect::BeginPlay()
{
	Super::BeginPlay();
	
    this->SphereComponent->OnComponentBeginOverlap.AddDynamic(
        this, &ADamageTrapAbilityEffect::OnComponentBeginOverlap);

    SetLifeSpan(Duration);
    SphereComponent->SetSphereRadius(TrapRadius);

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


void ADamageTrapAbilityEffect::OnComponentBeginOverlap(
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
            DisableTrap();
            DamageToShapeArea();
            SpawnHitParticle();
            PlaySound();
            Destroy();
        }
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
    UStateManagerComponent* StatsManagerComp =
        Cast<UStateManagerComponent>(InActor->GetComponentByClass(UStateManagerComponent::StaticClass()));

    if (StatsManagerComp->IsValidLowLevel())
    {
        if (StatsManagerComp->GetActivityValue(EActivity::IsImmortal))
        {
            return true;
        }
    }

    return false;
}