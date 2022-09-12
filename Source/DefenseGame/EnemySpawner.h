// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyDino;

UCLASS()
class DEFENSEGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void SpawnEnemy();

private:
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemyDino> EnemyDinoClass;
};
