// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/AbilityComponent.h"
#include "GameFramework/Character.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "Animation/AnimMontage.h"
#include "Interfaces/AbilityInterface.h"
#include "GameCore/GameUtils.h"

class UAbilityComponent;
class ACharacter;
class UTexture2D;
class UMaterialInterface;
class UAnimMontage;

// Sets default values
AAbilityBase::AAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    static UTexture2D* LoadedTextureObject = GameUtils::LoadAssetObject<UTexture2D>(
        TEXT("/Game/DynamicCombatSystem/Widgets/Textures/T_AbilityCrosshair"));

    CrosshairTexture = LoadedTextureObject;

    bIncludeOwnerDamage = true;
    OwnerDamageScalar = 1.0f;
    ImpulsePower = 20000.0f;

    LeftHandSocket = TEXT("magic_left_hand");
    RightHandSocket = TEXT("magic_right_hand");
    BeamSocket = TEXT("magic_beam");

    IndicatorRadius = 250.0f;

    static UMaterialInterface* LoadedMaterialObject = GameUtils::LoadAssetObject<UMaterialInterface>(
        TEXT("/Game/DynamicCombatSystem/VFX/Materials/MI_SpellIndicatror_01"));
    IndicatorMaterial = LoadedMaterialObject;

    bRotateOnPressed = true;
}

void AAbilityBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    CrosshairTexture = nullptr;
    IndicatorMaterial = nullptr;
    AbilityMontages.Empty();
}

void AAbilityBase::NativeInit(UAbilityComponent* InAbilityComponent)
{
    AbilityComponent = InAbilityComponent;
    if (AbilityComponent.IsValid())
    {
        Character = AbilityComponent->GetCharacter();
    }
}

void AAbilityBase::ConsumeManaAmount(float Amount)
{
    if (AbilityComponent.IsValid())
    {
        AbilityComponent->ConsumeMana(Amount);
    }
}

void AAbilityBase::ConsumeMana()
{
    ConsumeManaAmount(ManaCost);
}

void AAbilityBase::ShowIndicator(FVector Location)
{
    if (AbilityComponent.IsValid())
    {
        AbilityComponent->ShowSpellIndicator(Location, IndicatorRadius, IndicatorMaterial);
    }
}

void AAbilityBase::UpdateIndicator(FVector NewLocation)
{
    if (AbilityComponent.IsValid())
    {
        AbilityComponent->UpdateSpellIndicatorLocation(NewLocation);
    }
}

void AAbilityBase::HideIndicator()
{
    if (AbilityComponent.IsValid())
    {
        AbilityComponent->HideSpellIndicator();
    }
}

float AAbilityBase::PlayAbilityMontage(
    UAnimMontage* Montage, FName StartSectionName, bool bUseCastingSpeed, float PlayRate)
{
    if (AbilityComponent.IsValid())
    {
        return AbilityComponent->PlayAbilityMontage(
            Montage,
            bUseCastingSpeed ? GetCastingSpeed() : PlayRate,
            StartSectionName);
    }
    else
    {
        return 0.0f;
    }    
}

float AAbilityBase::GetDamage() const
{
    if (AbilityComponent.IsValid())
    {
        if (bIncludeOwnerDamage) 
        {
            IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(GetOwner());
            if (AbilityInterface != nullptr)
            {
                return Damage + OwnerDamageScalar * AbilityInterface->GetMagicDamage();
            }
        }
    }
    return Damage;
}

UAnimMontage* AAbilityBase::GetAbilityMontage(int Index) const
{
    return AbilityMontages[Index];
}

void AAbilityBase::ApplyImpulseToCharacter(AActor* Actor, FVector HitNormal, float Impulse)
{
    ACharacter* TargetCharacter = Cast<ACharacter>(Actor);

    if (GameUtils::IsValid(TargetCharacter))
    {
        if (TargetCharacter->GetMesh()->IsAnySimulatingPhysics())
        {
            TargetCharacter->GetMesh()->AddImpulse((HitNormal * -1) * Impulse); 
        }
    }
}

bool AAbilityBase::IsInHeightRange(FVector Location, float MaxHeightDeviation) const
{
    if (!Character.IsValid()) 
    {
        return false;
    }

    FVector ActorPos = Character->GetActorLocation();
    float CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    return FMath::Abs((ActorPos.Z - CapsuleHalfHeight) - Location.Z) <= MaxHeightDeviation;
}

float AAbilityBase::GetCastingSpeed() const
{
    IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(GetOwner());

    if (AbilityInterface != nullptr)
    {
        float Value = AbilityInterface->GetCastingSpeed();

        if (Value > 0.0f)
        {

            return Value;
        }
    }

    return 1.0f;
}

bool AAbilityBase::FindGroundLocation(FVector StartLocation, float GroundOffset, FVector& OutGroundLocation, FVector& OutNormal) const
{
    FVector EndLocation = StartLocation - FVector(0, 0, 2000.0f);
    TArray<TEnumAsByte<EObjectTypeQuery>> Array 
    { 
        UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic) 
    };

    TArray<AActor*> IgnoreActors;
    FHitResult HitResult;

    bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
        GetWorld(), 
        StartLocation, 
        EndLocation, 
        Array, 
        false, 
        IgnoreActors, 
        EDrawDebugTrace::Type::None,
        HitResult, 
        true);

    if (bHit)
    {
        OutGroundLocation = HitResult.Location + FVector(0, 0, GroundOffset);
        OutNormal = HitResult.Normal;
        return true;
    }
    else
    {
        OutGroundLocation = StartLocation;
        OutNormal = FVector::ZeroVector;
        return false;
    }
}


