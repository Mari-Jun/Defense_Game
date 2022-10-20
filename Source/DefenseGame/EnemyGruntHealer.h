// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyGruntHealer.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API AEnemyGruntHealer : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyGruntHealer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void KillObject() override;
};
