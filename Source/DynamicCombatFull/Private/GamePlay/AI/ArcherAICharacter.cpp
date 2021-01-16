// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAICharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Sound/SoundBase.h"
#include "Engine/DataTable.h"
#include "Components/SceneComponent.h"
#include "Components/EquipmentComponent.h"
#include "Components/StateManagerComponent.h"
#include "Components/StatsManagerComponent.h"
#include "Components/MontageManagerComponent.h"
#include "GamePlay/AI/BaseAIController.h"
#include "GameCore/GameUtils.h"

AArcherAICharacter::AArcherAICharacter() 
    : ArrowInitialSpeed(3500.0f)
{
    static UBehaviorTree* LoadedObject =
        GameUtils::LoadAssetObject<UBehaviorTree>("/Game/DynamicCombatSystem/Blueprints/AI/Archer/ArcherAIBT");
    BTree = LoadedObject;

    static USoundBase* LoadedDrawBowSoundObject =
        GameUtils::LoadAssetObject<USoundBase>("/Game/DynamicCombatSystem/SFX/CUE/Cue_DrawBow");
    DefaultDrawBowSoundObject = LoadedDrawBowSoundObject;

    static UDataTable* LoadedDataTable =
        GameUtils::LoadAssetObject<UDataTable>("/Game/DynamicCombatSystem/DataTables/AIArcherMontages");
    Montages = LoadedDataTable;

    ArrowSpawnLocation = CreateDefaultSubobject<USceneComponent>("ArrowSpawnLocation");
    ArrowSpawnLocation->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

    FString ObjectItemDir("/Game/DynamicCombatSystem/Blueprints/Items/ObjectItems/Instances/");
    auto SteelHelmetBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("SteelHelmetBP"));
    auto ElvenBowBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("ElvenBowBP"));
    auto ElvenArrowBPClass = GameUtils::LoadAssetClass<UItemBase>(ObjectItemDir + FString("ElvenArrowBP"));


    // for archer
    Equipment->SetEquipmentSlots({
        FEquipmentSlots(EItemType::Spell, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(),
                FStoredItem(),
                FStoredItem()
            },
            0, false)
        }),

        FEquipmentSlots(EItemType::Shield, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),

        FEquipmentSlots(EItemType::Head, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem(FGuid::NewGuid(), SteelHelmetBPClass, 1)}, 0, false)
        }),
        FEquipmentSlots(EItemType::Top, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Legs, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Hands, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Feet, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{FStoredItem()}, 0, false)
        }),
        FEquipmentSlots(EItemType::Arrows, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(FGuid::NewGuid(), ElvenArrowBPClass, 999)
            },
            0, false)
        }),
        FEquipmentSlots(EItemType::Tool, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(),
                FStoredItem()
            },
            0, false)
        }),
        FEquipmentSlots(EItemType::MeleeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem()
            },
            0, false)
        }),
        FEquipmentSlots(EItemType::RangeWeapon, TArray<FEquipmentSlot> {
            FEquipmentSlot(TArray<FStoredItem>{
                FStoredItem(FGuid::NewGuid(), ElvenBowBPClass, 1)
            },
            0, false)
        })
        });
}


void AArcherAICharacter::BeginPlay()
{
    Super::BeginPlay();

    Equipment->SetMainHandType(EItemType::RangeWeapon);

    StateManager->OnActivityChanged.AddDynamic(this, &AArcherAICharacter::OnActivityChanged);
}

void AArcherAICharacter::EndPlay(const EEndPlayReason::Type EndPlayResult)
{
    Super::EndPlay(EndPlayResult);
    StateManager->OnActivityChanged.RemoveDynamic(this, &AArcherAICharacter::OnActivityChanged);
}

void AArcherAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateAimAlpha(DeltaTime);
}

void AArcherAICharacter::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void AArcherAICharacter::UpdateAimAlpha(float DeltaTime)
{
    if (StateManager->GetState() == EState::Idle &&
        StateManager->GetActivityValue(EActivity::IsAimingPressed))
    {
        float Value = StatsManager->GetStatValue(EStat::AttackSpeed, true);
        AimAlpha = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 1.0f, DeltaTime, Value);
    }
    else
    {
        AimAlpha = UKismetMathLibrary::FInterpTo_Constant(AimAlpha, 0.0f, DeltaTime, 3.0f);
    }
}

bool AArcherAICharacter::DoesHoldBowString() const
{
    return
        StateManager->GetState() == EState::Idle &&
        !GetCharacterMovement()->IsFalling() &&
        Equipment->GetCombatType() == ECombatType::Range &&
        Equipment->IsInCombat() &&
        !GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

}

float AArcherAICharacter::BowAttack()
{
    if (CanBowAttack())
    {
        FStoredItem Item = Equipment->GetActiveItem(EItemType::Arrows, 0);
        SpawnArrowProjectile(Item);
        StateManager->SetState(EState::Attacking);
        UAnimMontage* AnimMontage = MontageManager->GetMontageForAction(EMontageAction::ShootArrow, 0);

        if (GameUtils::IsValid(AnimMontage))
        {
            float Duration = PlayAnimMontage(AnimMontage);
            StateManager->ResetState(Duration);
            return Duration;
        }
        else
        {
            StateManager->ResetState(0.0f);
        }
    }

    return 0.0f;
}

FTransform AArcherAICharacter::GetSpawnArrowTransform() const
{
    if (GameUtils::IsValid(AIController))
    {
        FVector ArrowSpawnLoc = ArrowSpawnLocation->GetComponentLocation();
        FVector ActorLoc = AIController->GetTarget()->GetActorLocation();

        float X = ActorLoc.X + UKismetMathLibrary::RandomFloatInRange(-35.0f, 35.0f);
        float Y = ActorLoc.Y + UKismetMathLibrary::RandomFloatInRange(-35.0f, 35.0f);
        float Z = ActorLoc.Z + UKismetMathLibrary::RandomFloatInRange(-25.0f, 50.0f);

        FVector A(X, Y, Z);

        float Distance = GetDistanceTo(AIController->GetTarget());

        FVector Vel = AIController->GetTarget()->GetVelocity()* (Distance / ArrowInitialSpeed);

        FVector Start = ArrowSpawnLoc;
        FVector Target = A + Vel;

        FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Start, Target);

        return FTransform(Rot, ArrowSpawnLoc);
    }
    else
    {
        return FTransform();
    }
}

void AArcherAICharacter::OnActivityChanged(EActivity InActivity, bool bValue)
{
    if (InActivity == EActivity::IsAimingPressed)
    {
        if (bValue)
        {
            UGameplayStatics::SpawnSoundAttached(DefaultDrawBowSoundObject, GetMesh());
        }
    }
}
