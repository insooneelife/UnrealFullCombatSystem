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
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

    Character = Cast<ACharacter>(GetOwner());
    if (Character.IsValid())
    {
        EquipmentComponent = Cast<UEquipmentComponent>(
            Character->GetComponentByClass(UEquipmentComponent::StaticClass()));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Character is not valid!"));
    }

    if (EquipmentComponent.IsValid())
    {
        EquipmentComponent->OnActiveItemChanged.AddDynamic(this, &UAbilityComponent::OnActiveItemChanged);
        EquipmentComponent->OnMainHandTypeChanged.AddDynamic(this, &UAbilityComponent::OnMainHandTypeChanged);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EquipmentComponent is not valid!"));
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

    if (EquipmentComponent.IsValid())
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
    if (SpellIndicator.IsValid())
    {
        SpellIndicator->SetActorLocation(InNewLocation);
    }
}

bool UAbilityComponent::StartAbility()
{
    if (IsCurrentAbilityValid())
    {
        TWeakObjectPtr<AActor> OwnerPtr(GetOwner());
        IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(OwnerPtr.Get());

        if (AbilityInterface != nullptr)
        {
            bool bCanCastAbility = AbilityInterface->CanCastAbility();

            if (bCanCastAbility)
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
    const FStoredItem& InOldItem, const FStoredItem& InNewItem, EItemType InType, int InSlotIndex, int InActiveIndex)
{
    if (InType == EItemType::None || 
        (EquipmentComponent.IsValid() && EquipmentComponent->GetSelectedMainHandType() == InType))
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

            SpawnAbility(InAbilityClass);
            AbilityChanged();
        }
    }
    else
    {
        SpawnAbility(InAbilityClass);
        AbilityChanged();
    }    
}

void UAbilityComponent::SpawnAbility(TSubclassOf<AAbilityBase> InAbilityClass)
{
    if (UKismetSystemLibrary::IsValidClass(InAbilityClass))
    {
        TWeakObjectPtr<AActor> OwnerPtr(GetOwner());

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        Params.Owner = OwnerPtr.Get();
        Params.Instigator = nullptr;

        AAbilityBase* Ability = GetWorld()->SpawnActor<AAbilityBase>(InAbilityClass, FTransform::Identity, Params);
        Ability->Init(this);

        CurrentAbility = Ability;
    }
    else
    {
        CurrentAbility = nullptr;
    }
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


void UAbilityComponent::ShowSpellIndicator(FVector InLocation, float InRadius, UMaterialInterface* const InMaterial)
{
    if (SpellIndicator.IsValid())
    {
        UpdateSpellIndicatorLocation(InLocation);

        SpellIndicator->SetRadius(InRadius);
        SpellIndicator->SetMaterial(InMaterial);
        SpellIndicator->Show();
    }
    else
    {
        TWeakObjectPtr<AActor> OwnerPtr(GetOwner());

        if (OwnerPtr.IsValid())
        {
            FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
            SpawnParameters.Owner = OwnerPtr.Get();
            SpawnParameters.Instigator = TWeakObjectPtr<APawn>(OwnerPtr.Get()->GetInstigator()).Get();

            TWeakObjectPtr<ASpellIndicatorActor> SpawnedActor =
                GetWorld()->SpawnActor<ASpellIndicatorActor>(
                    SpellIndicatorClass,
                    FTransform(FQuat::Identity, InLocation),
                    SpawnParameters);

            if (SpawnedActor.IsValid())
            {
                SpawnedActor->Init(InRadius, InMaterial);
                SpawnedActor->Show();
                SpellIndicator = SpawnedActor.Get();
            }
            
        }
    }
}

void UAbilityComponent::HideSpellIndicator()
{
    if (SpellIndicator.IsValid())
    {
        SpellIndicator->Hide();
    }
}

float UAbilityComponent::PlayAbilityMontage(UAnimMontage* const InMontage, float InPlayRate, FName InSection)
{
    if (Character.IsValid())
    {
        float Duration = Character->PlayAnimMontage(InMontage, InPlayRate, InSection);
        return Duration;
    }
    return 0.0f;    
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
    return CurrentAbility.IsValid();
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
    if (Character.IsValid())
    {
        UAnimMontage* Montage = Character->GetCurrentMontage();
        return RecentlyPlayedMontage.IsValid() && Montage == RecentlyPlayedMontage;
    }

    return false;
}


void UAbilityComponent::UpdateAbilityFromEquipment()
{
    if (!EquipmentComponent.IsValid())
    {
        return;
    }

    EItemType MainType = EquipmentComponent->GetSelectedMainHandType();
    TSubclassOf<UItemBase> ItemClass = EquipmentComponent->GetActiveItem(MainType, 0).ItemClass;

    if (UKismetSystemLibrary::IsValidClass(ItemClass.Get()))
    {
        TWeakObjectPtr<AActor> OwnerPtr(GetOwner());

        if (OwnerPtr.IsValid())
        {
            UItemBase* ItemBase = NewObject<UItemBase>(OwnerPtr.Get(), ItemClass);
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
    OnAbilityChanged.Broadcast(CurrentAbility.Get());
}

void UAbilityComponent::StopAbilityMontage()
{
    if (Character.IsValid())
    {
        UAnimMontage* AnimMontage = Character->GetCurrentMontage();

        if (RecentlyPlayedMontage.IsValid() && AnimMontage == RecentlyPlayedMontage)
        {
            Character->StopAnimMontage(AnimMontage);
        }
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

