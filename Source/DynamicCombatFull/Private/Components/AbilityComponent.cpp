// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/EquipmentComponent.h"
#include "GamePlay/Abilities/AbilityBase.h"
#include "GamePlay/SpellIndicatorActor.h"
#include "Interfaces/ItemHasAbility.h"
#include "Interfaces/AbilityInterface.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    bUpdateEquipmentAbility = true;
    SpawnIndicatorClass =
        GameUtils::LoadAssetClass<ASpellIndicatorActor>(TEXT("/Game/DynamicCombatSystem/Blueprints/BP_SpellIndicator"));
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

    Character = Cast<ACharacter>(GetOwner());

    EquipmentComponent = Cast<UEquipmentComponent>(GetOwner()->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (EquipmentComponent->IsValidLowLevel())
    {
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UAbilityComponent::OnActiveItemChanged);
        EquipmentComponent->OnMainHandTypeChanged.AddDynamic(this, &UAbilityComponent::OnMainHandTypeChanged);
    }

}

void UAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        CurrentAbility->Destroy();
    }
}


void UAbilityComponent::OnActiveItemChanged(FStoredItem OldItem, FStoredItem NewItem, EItemType Type, int SlotIndex, int ActiveIndex)
{
    if (Type == EItemType::None || EquipmentComponent->GetSelectedMainHandType() == Type)
    {
        UpdateAbilityFromEquipment();
    }
}

void UAbilityComponent::OnMainHandTypeChanged(EItemType Type)
{
    UpdateAbilityFromEquipment();
}

void UAbilityComponent::UpdateAbilityFromEquipment()
{
    EItemType MainType = EquipmentComponent->GetSelectedMainHandType();
    TSubclassOf<UItemBase> ItemClass = EquipmentComponent->GetActiveItem(MainType, 0).ItemClass;

    if (UKismetSystemLibrary::IsValidClass(ItemClass.Get()))
    {
        UItemBase* ItemBase = NewObject<UItemBase>(GetOwner(), ItemClass);

        IItemHasAbility* ItemHasAbility = Cast<IItemHasAbility>(ItemBase);
        if (ItemHasAbility != nullptr)
        {
            UpdateAbility(ItemHasAbility->GetAbility());
        }
        else
        {
            UpdateAbility(nullptr);
        }
    }
    else
    {
        UpdateAbility(nullptr);
    }
}

void UAbilityComponent::AbilityPressed()
{
    if (IsAbilityValid())
    {
        SetIsPressed(true);

        if (CurrentAbility->IsValidLowLevel())
        {
            CurrentAbility->Pressed();
        }
    }
}

void UAbilityComponent::AbilityReleased()
{
    if (GetIsPressed())
    {
        SetIsPressed(false);

        if (CurrentAbility->IsValidLowLevel())
        {
            CurrentAbility->Released();
        }
    }
}


void UAbilityComponent::AbilityEffect()
{
    if(IsUsingAbility())
    {
        CurrentAbility->Effect();
    }
}

void UAbilityComponent::ConsumeMana(float Amount)
{
    OnManaConsumed.Broadcast(Amount);
}

void UAbilityComponent::AbilityChanged()
{
    if (CurrentAbility->IsValidLowLevel())
    {
    }
    else
    {
        SetIsCasting(false);
        OnAbilityEnded.Broadcast(EAbilityEndResult::Canceled);
    }
    OnAbilityChanged.Broadcast(CurrentAbility);
}


void UAbilityComponent::ShowSpellIndicator(FVector Location, float Radius, UMaterialInterface* Material)
{
    if (SpellIndicator->IsValidLowLevel())
    {
        UpdateSpellIndicatorLocation(Location);

        SpellIndicator->SetRadius(Radius);
        SpellIndicator->SetMaterial(Material);

    }
    else
    {
        FActorSpawnParameters SpawnParameters = FActorSpawnParameters();

        SpawnParameters.Owner = GetOwner();
        SpawnParameters.Instigator = GetOwner()->GetInstigator();

        ASpellIndicatorActor* SpawnedActor =
            GetWorld()->SpawnActor<ASpellIndicatorActor>(
                SpawnIndicatorClass, FTransform(FQuat::Identity, Location), SpawnParameters);

        SpawnedActor->SetRadius(Radius);
        SpawnedActor->SetMaterial(Material);

        SpellIndicator = SpawnedActor;
    }

    SpellIndicator->Show();
}

void UAbilityComponent::HideSpellIndicator()
{
    if (SpellIndicator->IsValidLowLevel())
    {
        SpellIndicator->Hide();
    }
}

void UAbilityComponent::UpdateSpellIndicatorLocation(FVector NewLocation)
{
    if (SpellIndicator->IsValidLowLevel())
    {
        SpellIndicator->SetActorLocation(NewLocation);
    }
}

void UAbilityComponent::UpdateAbility(TSubclassOf<AAbilityBase> AbilityClass)
{
    if (CurrentAbility->IsValidLowLevel())
    {
        if (CurrentAbility->GetClass() != AbilityClass)
        {
            EndAbility(EAbilityEndResult::Destroyed);
            CurrentAbility->Destroy();
        }
    }

    if (AbilityClass->IsValidLowLevel())
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        Params.Owner = GetOwner();
        Params.Instigator = nullptr;

        AAbilityBase* Ability = GetWorld()->SpawnActor<AAbilityBase>(AbilityClass, FTransform::Identity, Params);
        Ability->SetAbilityComponent(this);

        CurrentAbility = Ability;
    }
    else
    {
        CurrentAbility = nullptr;
    }

    AbilityChanged();
}

bool UAbilityComponent::StartAbility()
{
    if (CurrentAbility->IsValidLowLevel())
    {
        IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(GetOwner());

        if (AbilityInterface != nullptr)
        {
            if (AbilityInterface->CanCastAbility())
            {
                if (GetIsCasting())
                {
                    return false;
                }
                else
                {
                    SetIsCasting(true);
                    OnAbilityStarted.Broadcast();

                    return true;
                }
            }
        }
    }
    
    return false;
}

float UAbilityComponent::PlayAbilityMontage(UAnimMontage* Montage, float PlayRate, FName Section)
{
    float Duration = Character->PlayAnimMontage(nullptr, PlayRate, Section);
    return Duration;
}

void UAbilityComponent::StopAbilityMontage()
{
    UAnimMontage* AnimMontage = Character->GetCurrentMontage();

    if (AnimMontage == RecentlyPlayedMontage)
    {
        Character->StopAnimMontage(AnimMontage);
    }
}

void UAbilityComponent::EndAbility(EAbilityEndResult Result)
{
    if (IsUsingAbility())
    {
        if (Result == EAbilityEndResult::Finished)
        {
            HideIndicatorIfNotPressed();
            SetIsCasting(false);
            CallAbilityEnded(Result);
        }
        else if (Result == EAbilityEndResult::Interrupted)
        {
            HideIndicatorIfNotPressed();
            SetIsCasting(false);
            StopAbilityMontage();
            CallAbilityEnded(Result);
        }
        else if (Result == EAbilityEndResult::Canceled)
        {
            if ((GetIsCasting() && CanAbilityBeCancelled()) || !GetIsCasting())
            {
                SetIsPressed(false);
                SetIsCasting(false);
                HideSpellIndicator();
                StopAbilityMontage();
                CallAbilityEnded(Result);
            }

        }
        else if (Result == EAbilityEndResult::OutOfMana)
        {
            SetIsPressed(false);
            HideSpellIndicator();
            SetIsCasting(false);
            CallAbilityEnded(Result);
        }
        else if (Result == EAbilityEndResult::Destroyed)
        {
            HideSpellIndicator();
            SetIsCasting(false);
            CallAbilityEnded(Result);
        }
    }
}

AAbilityBase* UAbilityComponent::GetCurrentAbility() const
{
    return CurrentAbility;
}

bool UAbilityComponent::IsAbilityUsingCrosshair() const
{
    if (CurrentAbility->IsValidLowLevel())
    {
        return CurrentAbility->IsUsingCrosshair();
    }
    else
    {
        return false;
    }
}

FTransform UAbilityComponent::GetEffectTransform() const
{
    if (CurrentAbility->IsValidLowLevel())
    {
        return CurrentAbility->GetEffectTransform();
    }
    else
    {
        return FTransform::Identity;
    }
}

bool UAbilityComponent::GetIsCasting() const
{
    return bIsCasting;
}

bool UAbilityComponent::GetIsPressed() const
{
    return bIsPressed;
}

bool UAbilityComponent::IsUsingAbility() const
{
    return GetIsPressed() || GetIsCasting();
}

float UAbilityComponent::GetManaCost() const
{
    if (CurrentAbility->IsValidLowLevel())
    {
        return CurrentAbility->GetManaCost();
    }
    else
    {
        return 0.0f;
    }
}

bool UAbilityComponent::CanAbilityBeCancelled() const
{
    if (CurrentAbility->IsValidLowLevel())
    {
        return CurrentAbility->CanBeCancelled();
    }
    else
    {
        return true;
    }
}

bool UAbilityComponent::IsAbilityValid() const
{
    return UKismetSystemLibrary::IsValid(CurrentAbility);
}

UTexture2D* UAbilityComponent::GetAbilityCrosshair() const
{
    if (CurrentAbility->IsValidLowLevel())
    {
        return CurrentAbility->GetCrosshairTexture();
    }
    else
    {
        return nullptr;
    }
}

bool UAbilityComponent::ShouldRotateOnPressed() const
{
    if (CurrentAbility->IsValidLowLevel())
    {
        return CurrentAbility->IsRotateOnPressed();
    }

    return false;
}

void UAbilityComponent::SetIsPressed(bool bValue)
{
    if (GetIsPressed() != bValue)
    {
        bIsPressed = bValue;

        OnPressedChanged.Broadcast(bIsPressed);
    }
}

void UAbilityComponent::SetIsCasting(bool bValue)
{
    if (GetIsCasting() != bValue)
    {
        bIsCasting = bValue;
        OnCastingChanged.Broadcast(bIsCasting);
    }
}

bool UAbilityComponent::IsPlayingAbilityMontage() const
{
    UAnimMontage* Montage = Character->GetCurrentMontage();

    return Montage == RecentlyPlayedMontage && RecentlyPlayedMontage->IsValidLowLevel();
}

void UAbilityComponent::HideIndicatorIfNotPressed()
{
    if (GetIsPressed())
    {
    }
    else
    {
        HideSpellIndicator();
    }
}

void UAbilityComponent::CallAbilityEnded(EAbilityEndResult Result)
{
    if (CurrentAbility->IsValidLowLevel())
    {
        CurrentAbility->Ended(Result);
    }

    OnAbilityEnded.Broadcast(Result);
}

