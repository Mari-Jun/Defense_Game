// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();

	GetEnemySpawner();

	WaitNextWave();
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveManager::GetEnemySpawner()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(Actor);
		if (EnemySpawner != nullptr)
		{
			EnemySpawners.Add(EnemySpawner, ESpawnerEmptyState::ENotEmpty);
			EnemySpawner->EmptySpawnedEnemyDelegate.AddDynamic(this, &AWaveManager::OnEmptySpawnedEnemy);
		}
	}
}

void AWaveManager::WaitNextWave()
{
	++CurrentWave;
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::StartWave, WaveWaitTime);
}

void AWaveManager::StartWave()
{
	for (auto& [EnemySpanwer, EmptyEnemy] : EnemySpawners)
	{
		EnemySpanwer->SpawnEnemys(CurrentWave);
		EmptyEnemy = ESpawnerEmptyState::ENotEmpty;
	}
}

void AWaveManager::OnEmptySpawnedEnemy(AEnemySpawner* EnemySpawner)
{
	EnemySpawners[EnemySpawner] = ESpawnerEmptyState::EEmpty;
	bool AllEmpty = true;
	for (const auto& [EnemySpanwer, EmptyEnemy] : EnemySpawners)
	{
		if (EmptyEnemy == ESpawnerEmptyState::ENotEmpty)
		{
			AllEmpty = false;
			break;
		}
	}

	if (AllEmpty)
	{
		if (CurrentWave == MaxWave)
		{
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Magenta, TEXT("Finish Game!!!"));
		}
		else
		{
			WaitNextWave();
		}
	}
}

