// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "EnemySpawner.generated.h"

class AEnemyDino;
class AEnemy;

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
struct FEnemySpawnTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	TArray<FSpawnEnemyInfo> SpawnEnemys;
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

private:
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemySpawnTable;
};
