#include "BTS_Base.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BlueprintNodeHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameCore/GameUtils.h"

UBTS_Base::UBTS_Base(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNodeInstance = true;


}

void UBTS_Base::SetOwner(AActor* InActorOwner)
{
    ActorOwner = InActorOwner;
    AIOwner = Cast<AAIController>(InActorOwner);

    UE_LOG(LogTemp, Error, TEXT("UBTS_Base  SetOwner  Owner : %s"),
        *GameUtils::GetDebugName(ActorOwner)
    );
}

/** called when node instance is added to tree */
void UBTS_Base::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
    UE_LOG(LogTemp, Error, TEXT("UBTS_Base  OnInstanceCreated  Owner : %s"),
        *GameUtils::GetDebugName(OwnerComp.GetOwner())
    );
}

/** called when node instance is removed from tree */
void UBTS_Base::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
    UE_LOG(LogTemp, Error, TEXT("UBTS_Base  OnInstanceDestroyed  Owner : %s"),
        *GameUtils::GetDebugName(OwnerComp.GetOwner())
    );
}

void UBTS_Base::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);
    ReceiveActivationAI(OwnerComp, AIOwner, AIOwner->GetPawn());
}

void UBTS_Base::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnCeaseRelevant(OwnerComp, NodeMemory);
    ReceiveDeactivationAI(OwnerComp, AIOwner, AIOwner->GetPawn());
}

void UBTS_Base::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
    Super::OnSearchStart(SearchData);
    ReceiveSearchStartAI(SearchData.OwnerComp, AIOwner, AIOwner->GetPawn());

    UE_LOG(LogTemp, Error, TEXT("OnSearchStart  OnInstanceCreated  AIOwner : %s   Pawn : %s"),
        *GameUtils::GetDebugName(AIOwner),
        *GameUtils::GetDebugName(AIOwner->GetPawn())
    );
}

void UBTS_Base::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    ReceiveTickAI(OwnerComp, AIOwner, AIOwner->GetPawn(), DeltaSeconds);
}


bool UBTS_Base::IsServiceActive() const
{
    UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
    const bool bIsActive = OwnerComp->IsAuxNodeActive(this);
    return bIsActive;
}