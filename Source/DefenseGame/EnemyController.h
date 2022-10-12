// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

class AEnemy;
class ABaseCharacter;
class ADefenseBase;

class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UAIPerceptionSystem;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class UAISenseConfig_Team;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void KilledControlledPawn();
	virtual void FindNearestDefenseBaseLocation();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void InitializePerception();
	UFUNCTION()
	virtual void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	void TriggerDamageEvent(float DamageAmount, AActor* DamageCauser);
	void TriggerTeamEvent(AActor* Actor);
	void LoseSense();
	
	void SetTargetCharacter(ABaseCharacter* Target);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	AEnemy* Enemy;

	UPROPERTY(BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UAIPerceptionSystem* PerceptionSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	float SightRadius = 3000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	float LoseSightRadius = 4500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	float VisionAngle = 120.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	float TeamSenseRange = 1000.f;

	bool SuccessSightSense = false;
	bool SuccessDamageSense = false;
	bool SuccessTeamSense = false;

	UAISenseConfig_Sight* SenseSight;
	UAISenseConfig_Damage* SenseDamage;
	UAISenseConfig_Team* SenseTeam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	float LoseSenseTime = 5.0f;
	FTimerHandle LoseSenseTimerHandle;

	ADefenseBase* TargetDefenseBase = nullptr;
	ABaseCharacter* TargetCharacter = nullptr;
	FVector TargetLocation;

	AActor* LastDamageCauser = nullptr;

protected:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	bool HasTarget() const { return TargetCharacter != nullptr || TargetDefenseBase != nullptr; }
	FVector GetAttackTargetLocation() const;
	ADefenseBase* GetTargetDefenseBase() const { return TargetDefenseBase; }
	ABaseCharacter* GetTargetCharacter() const { return TargetCharacter; }
	AActor* GetLastDamageCauser() const { return LastDamageCauser; }
};
