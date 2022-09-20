// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

class AEnemySpawner;

enum class ESpawnerEmptyState : uint8
{
	EEmpty, 
	ENotEmpty
};

UCLASS()
class DEFENSEGAME_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void GetEnemySpawner();
	void WaitNextWave();
	void StartWave();

	UFUNCTION()
	void OnEmptySpawnedEnemy(AEnemySpawner* EnemySpawner);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave", meta = (AllowPrivateAccess = "true"))
	int32 WaveWaitTime = 10;

	int32 CurrentWave = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave", meta = (AllowPrivateAccess = "true"))
	int32 MaxWave = 1;
	FTimerHandle WaveTimerHandle;

	/*bool 값은 이번 Wave에 소환한 Enemy들이 모두 죽었는지 여부*/
	TMap<AEnemySpawner*, ESpawnerEmptyState> EnemySpawners;
};
