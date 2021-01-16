// Fill out your copyright notice in the Description page of Project Settings.


#include "DissolveComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Materials/MaterialInstance.h"
#include "GameCore/GameUtils.h"

// Sets default values for this component's properties
UDissolveComponent::UDissolveComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bStartWithTickEnabled = false;
    
    static UMaterialInstance* LoadedObject = GameUtils::LoadAssetObject<UMaterialInstance>(
        TEXT("/Game/DynamicCombatSystem/Meshes/Materials/MI_DissolveEffect"));

    DissolveMaterial = LoadedObject;
    DissolveValueName = TEXT("amount");
    DissolveColorName = TEXT("color");
    DissolveInterpSpeed = 1.0f;
    DissolveColor = FLinearColor(5.0f, 0.0f, 0.0f, 0.0f);
}

void UDissolveComponent::StartDissolve(UPrimitiveComponent* InComponent, bool bInReversed)
{
    if (GameUtils::IsValid(InComponent))
    {
        int LComponentIndex = FindComponent(InComponent);
        if (LComponentIndex >= 0)
        {
            DissolvedComponents[LComponentIndex].bReverse = bInReversed;
            DissolvedComponents[LComponentIndex].bIsRunning = true;
        }
        else
        {
            // store component materials and replace them with dissolve material
            TArray<UMaterialInterface*> LMaterials;
            TArray<UMaterialInstanceDynamic*> LDissolveMaterials;

            for (int i = 0; i < InComponent->GetNumMaterials(); ++i)
            {
                LMaterials.Add(InComponent->GetMaterial(i));

                UMaterialInstanceDynamic* MatInstD = 
                    InComponent->CreateDynamicMaterialInstance(i, DissolveMaterial);

                MatInstD->SetVectorParameterValue(DissolveColorName, DissolveColor);
                LDissolveMaterials.Add(MatInstD);
            }

            // add to dissolve components
            FDissolvedComponent DissComp;
            DissComp.Component = InComponent;
            DissComp.Value = UKismetMathLibrary::Conv_BoolToFloat(bInReversed);
            DissComp.Materials = LMaterials;
            DissComp.DissolveMaterials = LDissolveMaterials;
            DissComp.bReverse = bInReversed;
            DissComp.bIsRunning = true;
            DissolvedComponents.Add(DissComp);
        }

        GetWorld()->GetTimerManager().SetTimer(
            DissolveComponentsTimerHandle, this, &UDissolveComponent::DissolveComponents, 0.016f, true);
    }
}

void UDissolveComponent::StopDissolve(UPrimitiveComponent* InComponent)
{
    int LIndex = FindComponent(InComponent);

    if (LIndex >= 0)
    {
        DissolvedComponents[LIndex].bIsRunning = false;
    }
}

void UDissolveComponent::DissolveComponents()
{
    bool bLKeepDissolving = false;
    for (int i = DissolvedComponents.Num() - 1; i >= 0; --i)
    {
        int LIndex = i;
        FDissolvedComponent DissComp = DissolvedComponents[i];

        if (GameUtils::IsValid(DissComp.Component))
        {
            if (DissComp.bIsRunning)
            {
                float Current = DissComp.Value;
                float Target = UKismetMathLibrary::Conv_BoolToFloat(DissComp.bReverse);
                float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

                float LNewValue =
                    UKismetMathLibrary::FInterpTo_Constant(Current, Target, DeltaTime, DissolveInterpSpeed);

                DissComp.Value = LNewValue;

                float A = LNewValue;
                float B = UKismetMathLibrary::Conv_BoolToFloat(!DissComp.bReverse);

                if (UKismetMathLibrary::NearlyEqual_FloatFloat(A, B, 0.0001))
                {
                    if (DissComp.bReverse)
                    {
                        RestoreComponentMaterials(LIndex);
                        OnDissolveFinished.Broadcast(DissComp.Component, DissComp.bReverse);

                        RemoveComponent(LIndex);
                        bLKeepDissolving = true;
                    }
                    else
                    {
                        DissComp.bIsRunning = false;
                    }
                }
                else
                {
                    bLKeepDissolving = true;

                    for (UMaterialInstanceDynamic* Mat : DissComp.DissolveMaterials)
                    {
                        Mat->SetScalarParameterValue(DissolveValueName, LNewValue);
                    }
                }
            }
        }
        else
        {
            RemoveComponent(LIndex);
        }
    }

    if (!bLKeepDissolving)
    {
        GetWorld()->GetTimerManager().ClearTimer(DissolveComponentsTimerHandle);
    }
}

void UDissolveComponent::RestoreComponentMaterials(int InIndex)
{
    UPrimitiveComponent* Component = DissolvedComponents[InIndex].Component;

    for (int i = 0; i < Component->GetNumMaterials(); ++i)
    {
        Component->SetMaterial(i, DissolvedComponents[InIndex].Materials[i]);
    }
}

void UDissolveComponent::RemoveComponent(int InIndex)
{
    DissolvedComponents.RemoveAt(InIndex);
}

int UDissolveComponent::FindComponent(UPrimitiveComponent* InComponent) const
{
    for (int i = 0; i < DissolvedComponents.Num(); ++i)
    {
        if (DissolvedComponents[i].Component == InComponent)
        {
            return i;
        }
    }

    return -1;
}