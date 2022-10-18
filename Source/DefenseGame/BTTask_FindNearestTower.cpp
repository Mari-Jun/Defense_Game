// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindNearestTower.h"
#include "EnemyController.h"
#include "DefenseBase.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_FindNearestTower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyController* Controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	if (Controller == nullptr || Controller->GetPawn() == nullptr) return EBTNodeResult::Failed;

	TArray<AActor*> BaseActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADefenseBase::StaticClass(), BaseActors);

	FVector EnemyLocation = Controller->GetPawn()->GetActorLocation();
	AActor* MinDefenseBase = nullptr;
	float MinLength = FLT_MAX;

	for (AActor* BaseActor : BaseActors)
	{
		float Length = (BaseActor->GetActorLocation() - EnemyLocation).Length();
		if (MinLength > Length)
		{
			MinDefenseBase = BaseActor;
			MinLength = Length;
		}
	}

	if (MinDefenseBase == nullptr) return EBTNodeResult::Failed;

	Controller->SetTargetDefenseBase(Cast<ADefenseBase>(MinDefenseBase));

	return EBTNodeResult::Succeeded;
}
