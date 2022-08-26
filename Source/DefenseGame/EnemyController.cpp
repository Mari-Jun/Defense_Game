// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyController::AEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("blackboard component");
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("behavior component");
	check(BehaviorTreeComponent);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr) return;

	Enemy = Cast<AEnemy>(InPawn);
	if (Enemy != nullptr)
	{
		if (Enemy->GetBehaviorTree() != nullptr)
		{
			BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
		}
	}
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (Enemy != nullptr)
	{
		RunBehaviorTree(Enemy->GetBehaviorTree());
		BehaviorTreeComponent->StartTree(*Enemy->GetBehaviorTree());
	}
}
