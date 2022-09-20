// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Enemy.h"
#include "EnemyController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnEnemys(1);
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemys(int WaveLevel)
{
	FString RowName = FString::Printf(TEXT("Wave%d"), WaveLevel);
	FEnemySpawnTable* SpawnDataRow = EnemySpawnTable->FindRow<FEnemySpawnTable>(FName(*RowName), "");
	if (SpawnDataRow != nullptr)
	{
		for (const auto& SpawnInfo : SpawnDataRow->SpawnEnemys)
		{
			for (int num = 0; num < SpawnInfo.NumOfEnemys; ++num)
			{
				GetWorld()->SpawnActor<AEnemy>(SpawnInfo.EnemyClass, GetActorTransform());
			}
		}
	}
}

