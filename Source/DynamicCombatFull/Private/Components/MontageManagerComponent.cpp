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

    IMontageManagerInterface* MontageManagerInterface = Cast<IMontageManagerInterface>(GetOwner());

    if (MontageManagerInterface == nullptr)
    {
        FString Name = UGameplayStatics::GetObjectClass(GetOwner())->GetDisplayNameText().ToString();
        UE_LOG(LogTemp, Error, TEXT("Does not implement interface IMontageManager!  %s"), *Name);
    }
}


UAnimMontage* UMontageManagerComponent::GetMontageForAction(EMontageAction Action, int Index)
{
    FMontageActionRow OutMontageAction;
    LastRequestedAction = Action;

    if (GetMontage(Action, OutMontageAction))
    {
        if (OutMontageAction.Montages[Index]->IsValidLowLevel())
        {
            return OutMontageAction.Montages[Index];
        }
    }
    return nullptr;
}

int UMontageManagerComponent::GetMontageActionLastIndex(EMontageAction Action) const
{
    FMontageActionRow OutMontageAction;
    if (GetMontage(Action, OutMontageAction))
    {
        if (OutMontageAction.Montages.Num() > 0)
        {
            return OutMontageAction.Montages.Num() - 1;
        }
    }
    
    return -1;
}

int UMontageManagerComponent::GetRandomMontageIndex(EMontageAction Action) const
{
    return UKismetMathLibrary::RandomIntegerInRange(0, GetMontageActionLastIndex(Action));
}

EMontageAction UMontageManagerComponent::GetLastRequestedAction() const
{
    return LastRequestedAction;
}

bool UMontageManagerComponent::GetMontage(EMontageAction Action, FMontageActionRow& OutMontageData) const
{
    IMontageManagerInterface* MontageManagerInterface = Cast<IMontageManagerInterface>(GetOwner());
    FString EnumStr = GameUtils::GetEnumDisplayNameAsString("EMontageAction", Action);
    UDataTable* DataTable = MontageManagerInterface->GetMontages(Action);
    FMontageActionRow* Item = DataTable->FindRow<FMontageActionRow>(FName(*EnumStr), FString(""));

    if (Item != nullptr)
    {
        OutMontageData = *Item;
        UE_LOG(LogTemp, Error, TEXT("Found!!!!!!!!!!"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Not Found!!!!!!!!!!"));
        return false;
    }
}

