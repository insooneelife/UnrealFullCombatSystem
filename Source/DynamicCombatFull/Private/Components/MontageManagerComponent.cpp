// Fill out your copyright notice in the Description page of Project Settings.


#include "MontageManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interfaces/MontageManagerInterface.h"
#include "GameCore/GameUtils.h"


// Sets default values for this component's properties
UMontageManagerComponent::UMontageManagerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;

}


// Called when the game starts
void UMontageManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}


UAnimMontage* UMontageManagerComponent::GetMontageForAction(EMontageAction InAction, int InIndex)
{
    FMontageActionRow OutMontageAction;
    LastRequestedAction = InAction;

    if (GetMontage(InAction, OutMontageAction))
    {
        UAnimMontage* AnimMontage = OutMontageAction.Montages[InIndex];
        if (GameUtils::IsValid(AnimMontage))
        {
            return AnimMontage;
        }
    }
    return nullptr;
}

int UMontageManagerComponent::GetMontageActionLastIndex(EMontageAction InAction) const
{
    FMontageActionRow OutMontageAction;
    if (GetMontage(InAction, OutMontageAction))
    {
        if (OutMontageAction.Montages.Num() > 0)
        {
            return OutMontageAction.Montages.Num() - 1;
        }
    }
    
    return -1;
}

int UMontageManagerComponent::GetRandomMontageIndex(EMontageAction InAction) const
{
    return UKismetMathLibrary::RandomIntegerInRange(0, GetMontageActionLastIndex(InAction));
}

EMontageAction UMontageManagerComponent::GetLastRequestedAction() const
{
    return LastRequestedAction;
}

bool UMontageManagerComponent::GetMontage(EMontageAction InAction, FMontageActionRow& OutMontageData) const
{
    IMontageManagerInterface* MontageManagerInterface = Cast<IMontageManagerInterface>(GetOwner());
    FString EnumStr = GameUtils::GetEnumDisplayNameAsString("EMontageAction", InAction);

    if (MontageManagerInterface != nullptr)
    {
        UDataTable* DataTable = MontageManagerInterface->GetMontages(InAction);
        FMontageActionRow* Item = DataTable->FindRow<FMontageActionRow>(FName(*EnumStr), FString(""));

        if (Item != nullptr)
        {
            OutMontageData = *Item;
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

