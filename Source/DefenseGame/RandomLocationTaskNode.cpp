// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomLocationTaskNode.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

URandomLocationTaskNode::URandomLocationTaskNode()
{
}

EBTNodeResult::Type URandomLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlledPawn == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlledPawn->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	const FVector Origin = ControlledPawn->GetActorLocation();
	FNavLocation PatrolLoction;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1000.f, PatrolLoction))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", PatrolLoction.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
