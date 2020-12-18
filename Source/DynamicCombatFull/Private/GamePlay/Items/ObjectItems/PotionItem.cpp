// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionItem.h"
#include "Components/ExtendedStatComponent.h"

UPotionItem::UPotionItem(const FObjectInitializer& ObjectInitializer)
{
    Type = EStat::Health;
    Value = 25.0f;

    Item = FItem(
        FName(TEXT("Base Potion")),
        FText::FromString(TEXT("Item description")),
        EItemType::Tool, true, true, true, nullptr);
}

void UPotionItem::UseItem(AActor* Caller)
{
    TArray<UActorComponent*> Comps;
    Caller->GetComponents(UExtendedStatComponent::StaticClass(), Comps);

    for (UActorComponent* Comp : Comps)
    {
        UExtendedStatComponent* StatComp = Cast<UExtendedStatComponent>(Comp);
        if (StatComp->IsValidLowLevel())
        {
            if (StatComp->GetStatType() == Type)
            {
                StatComp->ModifyStat(Value, false);
            }
        }
    }

}