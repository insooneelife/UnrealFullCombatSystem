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

    TSubclassOf<ASpellIndicatorActor> LoadedClass =
        GameUtils::LoadAssetClass<ASpellIndicatorActor>(TEXT("/Game/DynamicCombatSystem/Blueprints/SpellIndicatorActorBP"));
    SpawnIndicatorClass = LoadedClass;
        
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

    Character = Cast<ACharacter>(GetOwner());
    EquipmentComponent = Cast<UEquipmentComponent>(Character->GetComponentByClass(UEquipmentComponent::StaticClass()));

    if (GameUtils::IsValid(EquipmentComponent))
    {
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UAbilityComponent::OnActiveItemChanged);
        EquipmentComponent->OnMainHandTypeChanged.AddDynamic(this, &UAbilityComponent::OnMainHandTypeChanged);
    }

}

void UAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed)
    {
        if (IsCurrentAbilityValid())
        {
            CurrentAbility->Destroy();
        }
    }

    if (GameUtils::IsValid(EquipmentComponent))
    {
        EquipmentComponent->OnActiveItemChanged.RemoveDynamic(this, &UAbilityComponent::OnActiveItemChanged);
        EquipmentComponent->OnMainHandTypeChanged.RemoveDynamic(this, &UAbilityComponent::OnMainHandTypeChanged);
    }

    Super::EndPlay(EndPlayReason);
}

void UAbilityComponent::AbilityEffect()
{
    if(IsUsingAbility())
    {
        if (IsCurrentAbilityValid())
        {
            CurrentAbility->Effect();
        }
    }
}

void UAbilityComponent::UpdateSpellIndicatorLocation(FVector InNewLocation)
{
    if (SpellIndicator != nullptr)
    {
        SpellIndicator->SetActorLocation(InNewLocation);
    }
}

bool UAbilityComponent::StartAbility()
{
    if (IsCurrentAbilityValid())
    {
        IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(GetOwner());

        if (AbilityInterface != nullptr)
        {
            if (AbilityInterface->CanCastAbility())
            {
                if (IsCasting())
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

void UAbilityComponent::EndAbility(EAbilityEndResult InResult)
{
    if (IsUsingAbility())
    {
        if (InResult == EAbilityEndResult::Finished)
        {
            HideIndicatorIfNotPressed();
            SetIsCasting(false);
            CallAbilityEnded(InResult);
        }
        else if (InResult == EAbilityEndResult::Interrupted)
        {
            HideIndicatorIfNotPressed();
            SetIsCasting(false);
            StopAbilityMontage();
            CallAbilityEnded(InResult);
        }
        else if (InResult == EAbilityEndResult::Canceled)
        {
            if ((IsCasting() && CanAbilityBeCancelled()) || !IsCasting())
            {
                SetIsPressed(false);
                SetIsCasting(false);
                HideSpellIndicator();
                StopAbilityMontage();
                CallAbilityEnded(InResult);
            }

        }
        else if (InResult == EAbilityEndResult::OutOfMana)
        {
            SetIsPressed(false);
            HideSpellIndicator();
            SetIsCasting(false);
            CallAbilityEnded(InResult);
        }
        else if (InResult == EAbilityEndResult::Destroyed)
        {
            HideSpellIndicator();
            SetIsCasting(false);
            CallAbilityEnded(InResult);
        }
    }
}


bool UAbilityComponent::IsUsingAbility() const
{
    return IsPressed() || IsCasting();
}

bool UAbilityComponent::CanAbilityBeCancelled() const
{
    if (IsCurrentAbilityValid())
    {
        return CurrentAbility->CanBeCancelled();
    }
    else
    {
        return true;
    }
}

void UAbilityComponent::OnActiveItemChanged(
    FStoredItem InOldItem, FStoredItem InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex)
{
    if (InType == EItemType::None || EquipmentComponent->GetSelectedMainHandType() == InType)
    {
        UpdateAbilityFromEquipment();
    }
}

void UAbilityComponent::OnMainHandTypeChanged(EItemType InType)
{
    UpdateAbilityFromEquipment();
}

void UAbilityComponent::UpdateAbility(TSubclassOf<AAbilityBase> InAbilityClass)
{
    if (IsCurrentAbilityValid())
    {
        if (CurrentAbility->GetClass() != InAbilityClass)
        {
            EndAbility(EAbilityEndResult::Destroyed);
            CurrentAbility->Destroy();
        }
    }

    if (UKismetSystemLibrary::IsValidClass(InAbilityClass))
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        Params.Owner = GetOwner();
        Params.Instigator = nullptr;

        AAbilityBase* Ability = GetWorld()->SpawnActor<AAbilityBase>(InAbilityClass, FTransform::Identity, Params);
        Ability->Init(this);

        CurrentAbility = Ability;
    }
    else
    {
        CurrentAbility = nullptr;
    }

    AbilityChanged();
}

void UAbilityComponent::AbilityPressed()
{
    if (IsCurrentAbilityValid())
    {
        SetIsPressed(true);

        if (IsCurrentAbilityValid())
        {
            CurrentAbility->Pressed();
        }
    }
}

void UAbilityComponent::AbilityReleased()
{
    if (IsPressed())
    {
        SetIsPressed(false);
        if (IsCurrentAbilityValid())
        {
            CurrentAbility->Released();
        }
    }
}

void UAbilityComponent::ConsumeMana(float InAmount)
{
    OnManaConsumed.Broadcast(InAmount);
}


void UAbilityComponent::ShowSpellIndicator(FVector InLocation, float InRadius, UMaterialInterface* InMaterial)
{
    if (SpellIndicator != nullptr)
    {
        UpdateSpellIndicatorLocation(InLocation);

        SpellIndicator->SetRadius(InRadius);
        SpellIndicator->SetMaterial(InMaterial);
        SpellIndicator->Show();
    }
    else
    {
        FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
        SpawnParameters.Owner = GetOwner();
        SpawnParameters.Instigator = GetOwner()->GetInstigator();

        ASpellIndicatorActor* SpawnedActor =
            GetWorld()->SpawnActor<ASpellIndicatorActor>(
                SpawnIndicatorClass, FTransform(FQuat::Identity, InLocation), SpawnParameters);

        if (GameUtils::IsValid(SpawnedActor))
        {
            SpawnedActor->Init(InRadius, InMaterial);
            SpellIndicator = SpawnedActor;
            SpellIndicator->Show();
        }
    }
}

void UAbilityComponent::HideSpellIndicator()
{
    if (SpellIndicator != nullptr)
    {
        SpellIndicator->Hide();
    }
}

float UAbilityComponent::PlayAbilityMontage(UAnimMontage* InMontage, float InPlayRate, FName InSection)
{
    float Duration = Character->PlayAnimMontage(InMontage, InPlayRate, InSection);
    return Duration;
}

bool UAbilityComponent::IsAbilityUsingCrosshair() const
{
    if (IsCurrentAbilityValid())
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
    if (IsCurrentAbilityValid())
    {
        return CurrentAbility->GetEffectTransform();
    }
    else
    {
        return FTransform::Identity;
    }
}



float UAbilityComponent::GetManaCost() const
{
    if (IsCurrentAbilityValid())
    {
        return CurrentAbility->GetManaCost();
    }
    else
    {
        return 0.0f;
    }
}

bool UAbilityComponent::IsCurrentAbilityValid() const
{
    return CurrentAbility != nullptr;
}

UTexture2D* UAbilityComponent::GetAbilityCrosshair() const
{
    if (IsCurrentAbilityValid())
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
    if (IsCurrentAbilityValid())
    {
        return CurrentAbility->IsRotateOnPressed();
    }

    return false;
}

bool UAbilityComponent::IsPlayingAbilityMontage() const
{
    UAnimMontage* Montage = Character->GetCurrentMontage();
    return Montage == RecentlyPlayedMontage && GameUtils::IsValid(RecentlyPlayedMontage);
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


void UAbilityComponent::AbilityChanged()
{
    if (IsCurrentAbilityValid())
    {
    }
    else
    {
        SetIsCasting(false);
        OnAbilityEnded.Broadcast(EAbilityEndResult::Canceled);
    }
    OnAbilityChanged.Broadcast(CurrentAbility);
}

void UAbilityComponent::StopAbilityMontage()
{
    UAnimMontage* AnimMontage = Character->GetCurrentMontage();

    if (AnimMontage == RecentlyPlayedMontage)
    {
        Character->StopAnimMontage(AnimMontage);
    }
}

void UAbilityComponent::SetIsPressed(bool bInValue)
{
    if (IsPressed() != bInValue)
    {
        bIsPressed = bInValue;

        OnPressedChanged.Broadcast(bIsPressed);
    }
}

void UAbilityComponent::SetIsCasting(bool bInValue)
{
    if (IsCasting() != bInValue)
    {
        bIsCasting = bInValue;
        OnCastingChanged.Broadcast(bIsCasting);
    }
}

void UAbilityComponent::HideIndicatorIfNotPressed()
{
    if (IsPressed())
    {
    }
    else
    {
        HideSpellIndicator();
    }
}

void UAbilityComponent::CallAbilityEnded(EAbilityEndResult InResult)
{
    if (IsCurrentAbilityValid())
    {
        CurrentAbility->Ended(InResult);
    }

    OnAbilityEnded.Broadcast(InResult);
}

