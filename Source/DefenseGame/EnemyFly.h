// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyFly.generated.h"

class AProjectile;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API AEnemyFly : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyFly();

protected:
	virtual void BeginPlay() override;

public:
	virtual void KillObject() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> AttackProjectile;
};
