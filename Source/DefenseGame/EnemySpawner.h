// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "EnemySpawner.generated.h"

class AEnemyDino;
class AEnemy;
class AWaveManager;

class UBoxComponent;

USTRUCT(BlueprintType)
struct FSpawnEnemyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEnemy> EnemyClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 NumOfEnemys = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 20))
	int32 EnemyLevel = 1;
};

USTRUCT(BlueprintType)
struct FSpawnTimeEnemyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSpawnEnemyInfo> SpawnEnemys;
};

USTRUCT(BlueprintType)
struct FEnemySpawnTable : public FTableRowBase
{
	GENERATED_BODY()

	/**Time과 그 시간에 나오는 Enemy들 웨이브 시작후 최대 시간은 30으로 설정해주세요.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	TMap<int32, FSpawnTimeEnemyInfo> SpawnEnemys;
};

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

	void SpawnEnemys(int WaveLevel);

	UFUNCTION()
	void OnEnemyDead();

private:
	UPROPERTY()
	AWaveManager* WaveManager;

	FTimerHandle SpawnTimerHandle;
	int32 PreviousTime = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemySpawnTable;
	FEnemySpawnTable* CurrentWaveSpawnDataRow;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SpawnPoint;

public:
	void SetWaveManager(AWaveManager* Manager) { WaveManager = Manager; }
};
