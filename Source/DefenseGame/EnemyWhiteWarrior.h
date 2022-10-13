// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyWhiteWarrior.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API AEnemyWhiteWarrior : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyWhiteWarrior();

protected:
	virtual void BeginPlay() override;

public:
	virtual void AttackHitStart() override;
	virtual void AttackHitEnd() override;
	virtual void KillObject() override;

protected:
	UFUNCTION()
	virtual void OnAttackBoxOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* AttackParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DeathParticle;
};
