// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"
#include "DefenseBase.h"
#include "BaseCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Team.h"
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
			RunBehaviorTree(Enemy->GetBehaviorTree());
			BehaviorTreeComponent->StartTree(*Enemy->GetBehaviorTree());
		}
	}

	if (BlackboardComponent != nullptr)
	{
		BlackboardComponent->SetValueAsBool("LoseSense", true);
	}

	PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
	if (PerceptionSystem != nullptr)
	{
		SetGenericTeamId(Enemy->GetGenericTeamId());
		PerceptionSystem->UpdateListener(*GetPerceptionComponent());
	}

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnPerceptionUpdate);
}

void AEnemyController::KilledControlledPawn()
{
	if (Enemy != nullptr)
	{
		BehaviorTreeComponent->StopTree();
		GetWorld()->DestroyActor(this);
	}
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter != nullptr && TargetCharacter->GetCharacterState() == ECharacterState::EDeath)
	{
		SetTargetCharacter(nullptr);

		BlackboardComponent->SetValueAsBool("LoseSense", true);
	}
}

float AEnemyController::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (SenseDamage != nullptr && Enemy != nullptr)
	{
		TriggerDamageEvent(DamageAmount, DamageCauser);
	}

	LastDamageCauser = DamageCauser;

	return DamageAmount;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyController::InitializePerception()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("ai perception"));

	SenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>("sense sight");
	SenseSight->SightRadius = SightRadius;
	SenseSight->LoseSightRadius = LoseSightRadius;
	SenseSight->PeripheralVisionAngleDegrees = VisionAngle * 0.5f;
	SenseSight->DetectionByAffiliation.bDetectEnemies = true;

	GetPerceptionComponent()->ConfigureSense(*SenseSight);
	GetPerceptionComponent()->SetDominantSense(SenseSight->GetSenseImplementation());

	SenseDamage = CreateDefaultSubobject<UAISenseConfig_Damage>("sense damage");
	SenseDamage->SetMaxAge(5.0f);

	GetPerceptionComponent()->ConfigureSense(*SenseDamage);
}

void AEnemyController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Actor);
	if (BaseCharacter == nullptr) return;

	TSubclassOf<UAISense> AISense = PerceptionSystem->GetSenseClassForStimulus(GetWorld(), Stimulus);
	FString SenseName = AISense->GetName();

	if (Stimulus.WasSuccessfullySensed())
	{
		if (SenseName == UAISense_Damage::StaticClass()->GetName())
		{
			SuccessDamageSense = true;
		}
		else if (SenseName == UAISense_Sight::StaticClass()->GetName())
		{
			SuccessSightSense = true;
		}

		BlackboardComponent->SetValueAsBool("LoseSense", false);

		if (TargetCharacter == nullptr)
		{
			SetTargetCharacter(BaseCharacter);
		}
	}
	else
	{
		if (SenseName == UAISense_Damage::StaticClass()->GetName())
		{
			SuccessDamageSense = false;
		}
		else if (SenseName == UAISense_Sight::StaticClass()->GetName())
		{
			SuccessSightSense = false;
		}

		if (!SuccessDamageSense && !SuccessSightSense && TargetCharacter != nullptr)
		{
			if (SenseName == UAISense_Sight::StaticClass()->GetName())
			{
				TargetLocation = TargetCharacter->GetActorLocation();
			}
			GetWorldTimerManager().SetTimer(LoseSenseTimerHandle, this, &AEnemyController::LoseSense, LoseSenseTime, false);
			SetTargetCharacter(nullptr);
		}
	}
}

void AEnemyController::TriggerDamageEvent(float DamageAmount, AActor* DamageCauser)
{
	if (PerceptionSystem != nullptr)
	{
		FAIDamageEvent Event = FAIDamageEvent(Enemy, DamageCauser, DamageAmount,
			DamageCauser->GetActorLocation(), Enemy->GetActorLocation());
		PerceptionSystem->OnEvent(Event);
	}
}

void AEnemyController::LoseSense()
{
	if (!SuccessDamageSense && !SuccessSightSense)
	{
		BlackboardComponent->SetValueAsBool("LoseSense", true);
	}
}

void AEnemyController::SetTargetCharacter(ABaseCharacter* Target)
{
	TargetCharacter = Target;
	BlackboardComponent->SetValueAsObject("TargetCharacter", TargetCharacter);
	SetFocus(TargetCharacter);
	if (TargetCharacter != nullptr) GetNewTarget();
	else if (TargetCharacter == nullptr && TargetDefenseBase == nullptr) LoseTarget();
}

void AEnemyController::SetTargetDefenseBase(ADefenseBase* Target)
{
	TargetDefenseBase = Target;
	BlackboardComponent->SetValueAsVector("BaseTargetLocation", TargetDefenseBase->GetActorLocation());
	if (TargetDefenseBase != nullptr) GetNewTarget();
	else if (TargetCharacter == nullptr && TargetDefenseBase == nullptr) LoseTarget();
}

void AEnemyController::GetNewTarget()
{
	if (Enemy == nullptr) return;
	Enemy->GetNewTarget();
}

void AEnemyController::LoseTarget()
{
	if (Enemy == nullptr) return;
	Enemy->LoseTarget();
}

ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn == nullptr) { return ETeamAttitude::Neutral; }

	const IGenericTeamAgentInterface* OtherTI = Cast<IGenericTeamAgentInterface>(&Other);
	IGenericTeamAgentInterface* OtherControllerTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (OtherTI == nullptr && OtherControllerTI == nullptr) { return ETeamAttitude::Neutral; }

	FGenericTeamId OtherActorTeamId;
	if (OtherTI != nullptr)
	{
		OtherActorTeamId = OtherTI->GetGenericTeamId();
	}
	else if (OtherControllerTI != nullptr)
	{
		OtherActorTeamId = OtherControllerTI->GetGenericTeamId();
	}
	
	if (Enemy == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	FGenericTeamId ControllerId = Enemy->GetGenericTeamId();
	if (OtherActorTeamId == 8)
	{
		return ETeamAttitude::Neutral;
	}
	else if (OtherActorTeamId == ControllerId)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}

FVector AEnemyController::GetAttackTargetLocation() const
{
	if (TargetCharacter != nullptr) return TargetCharacter->GetActorLocation();
	if (TargetDefenseBase != nullptr) return TargetDefenseBase->GetActorLocation();
	return Enemy->GetActorLocation();
}
