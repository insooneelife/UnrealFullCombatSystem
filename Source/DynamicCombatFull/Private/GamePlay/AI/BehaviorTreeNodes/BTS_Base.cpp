#include "BTS_Base.h"
#include "AIController.h"
#include "BlueprintNodeHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameCore/GameUtils.h"

UBTS_Base::UBTS_Base(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bCreateNodeInstance = true;
    bNotifyBecomeRelevant = true;
    bNotifyCeaseRelevant = true;
}

void UBTS_Base::SetOwner(AActor* InActorOwner)
{
    ActorOwner = InActorOwner;
    AIOwner = Cast<AAIController>(InActorOwner);
}

/** called when node instance is added to tree */
void UBTS_Base::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
}

/** called when node instance is removed from tree */
void UBTS_Base::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
}

void UBTS_Base::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);
    if (AIOwner.IsValid())
    {
        ReceiveActivationAI(OwnerComp, AIOwner.Get(), AIOwner->GetPawn());
    }
}

void UBTS_Base::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnCeaseRelevant(OwnerComp, NodeMemory);
    if (AIOwner.IsValid())
    {
        ReceiveDeactivationAI(OwnerComp, AIOwner.Get(), AIOwner->GetPawn());
    }
}

void UBTS_Base::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
    Super::OnSearchStart(SearchData);
    if (AIOwner.IsValid())
    {
        ReceiveSearchStartAI(SearchData.OwnerComp, AIOwner.Get(), AIOwner->GetPawn());
    }
}

void UBTS_Base::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    if (AIOwner.IsValid())
    {
        ReceiveTickAI(OwnerComp, AIOwner.Get(), AIOwner->GetPawn(), DeltaSeconds);
    }
}


bool UBTS_Base::IsServiceActive() const
{
    UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
    const bool bIsActive = OwnerComp->IsAuxNodeActive(this);
    return bIsActive;
}