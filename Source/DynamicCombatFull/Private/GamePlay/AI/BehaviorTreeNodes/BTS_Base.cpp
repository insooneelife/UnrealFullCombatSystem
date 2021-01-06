#include "BTS_Base.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BlueprintNodeHelpers.h"
#include "BehaviorTree/BehaviorTree.h"

UBTS_Base::UBTS_Base(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{}

void UBTS_Base::SetOwner(AActor* InActorOwner)
{
    ActorOwner = InActorOwner;
    AIOwner = Cast<AAIController>(InActorOwner);
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