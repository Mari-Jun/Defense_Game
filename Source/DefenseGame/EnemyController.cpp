// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"
#include "DefenseBase.h"
#include "BaseCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyController::AEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("blackboard component");
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("behavior component");
	check(BehaviorTreeComponent);

	InitializePerception();
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

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnPerceptionUpdate);
}

void AEnemyController::KilledControlledPawn()
{
	if (Enemy != nullptr)
	{
		BehaviorTreeComponent->StopTree();
	}
}

FVector AEnemyController::FindNearestDefenseBaseLocation()
{
	if (Enemy == nullptr) return FVector{};
	
	TArray<AActor*> BaseActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADefenseBase::StaticClass(), BaseActors);

	FVector EnemyLocation = Enemy->GetActorLocation();
	FVector MinLocation = Enemy->GetActorLocation();
	float MinLength = FLT_MAX;

	for (AActor* BaseActor : BaseActors)
	{
		float Length = (BaseActor->GetActorLocation() - EnemyLocation).Length();
		if (MinLength > Length)
		{
			MinLocation = BaseActor->GetActorLocation();
			MinLength = Length;
		}
	}

	return MinLocation;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	if (Enemy != nullptr)
	{
		RunBehaviorTree(Enemy->GetBehaviorTree());
		BehaviorTreeComponent->StartTree(*Enemy->GetBehaviorTree());
	}

	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsVector("BaseTargetLocation", FindNearestDefenseBaseLocation());
	}
}

void AEnemyController::InitializePerception()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("ai perception"));

	if(UseSightSense)
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("sense sight");
		Sight->SightRadius = SightRadius;
		Sight->LoseSightRadius = LoseSightRadius;
		Sight->PeripheralVisionAngleDegrees = VisionAngle * 0.5f;
		Sight->SetMaxAge(LoseSightTime);
		Sight->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->ConfigureSense(*Sight);
		GetPerceptionComponent()->SetDominantSense(Sight->GetSenseImplementation());
	}
}

void AEnemyController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Actor);
	if (BaseCharacter == nullptr) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		TargetCharacter = BaseCharacter;
		SetFocus(BaseCharacter);
	}
	else
	{
		TargetCharacter = nullptr;
		SetFocus(nullptr);
		BlackboardComponent->SetValueAsVector("BaseTargetLocation", FindNearestDefenseBaseLocation());
	}
	BlackboardComponent->SetValueAsObject("TargetCharacter", TargetCharacter);
}