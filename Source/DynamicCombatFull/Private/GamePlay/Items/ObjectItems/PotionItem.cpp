// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionItem.h"
#include "Components/ExtendedStatComponent.h"
#include "GameCore/GameUtils.h"
#include "GamePlay/Items/DisplayedItems/DisplayedItem.h"

UPotionItem::UPotionItem(const FObjectInitializer& ObjectInitializer)
{
    Type = EStat::Health;
    Value = 25.0f;

    static UTexture2D* LoadTexture =
        GameUtils::LoadAssetObject<UTexture2D>("/Game/DynamicCombatSystem/Widgets/Textures/T_Tool");

    Item = FItem(
        FName(TEXT("Base Potion")),
        FText::FromString(TEXT("Item description")),
        EItemType::Tool, true, true, true, LoadTexture);

    UseMontage = GameUtils::LoadAssetObject<UAnimMontage>(
        "/Game/DynamicCombatSystem/Montages/Player/Common/M_Common_DrinkPotion");
}

void UPotionItem::UseItem(AActor* Caller)
{
    TArray<UActorComponent*> Comps;
    Caller->GetComponents(UExtendedStatComponent::StaticClass(), Comps);

    for (UActorComponent* Comp : Comps)
    {
        UExtendedStatComponent* StatComp = Cast<UExtendedStatComponent>(Comp);

        if (GameUtils::IsValid(StatComp))
        {
            if (StatComp->GetStatType() == Type)
            {
                StatComp->ModifyStat(Value, false);
            }
        }
    }

}