// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "WaveManager.h"
#include "Enemy.h"
#include "EnemyController.h"

#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnPoint = CreateDefaultSubobject<UBoxComponent>("Spawn Point");
	SetRootComponent(SpawnPoint);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnTimerHandle.IsValid())
	{
		float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(SpawnTimerHandle);
		int32 IntElapsedTime = FMath::FloorToInt32(ElapsedTime);

		if (CurrentWaveSpawnDataRow == nullptr) return;

		if (PreviousTime + 1 == IntElapsedTime)
		{
			PreviousTime = IntElapsedTime;

			if (CurrentWaveSpawnDataRow->SpawnEnemys.Contains(IntElapsedTime))
			{
				for (const auto& [Time, SpawnTimeInfo] : CurrentWaveSpawnDataRow->SpawnEnemys)
				{
					if (Time == IntElapsedTime)
					{
						for (const auto& SpawnInfo : SpawnTimeInfo.SpawnEnemys)
						{
							for (int num = 0; num < SpawnInfo.NumOfEnemys; ++num)
							{
								AEnemy* SpawnedEnemy = nullptr;
								while (SpawnedEnemy == nullptr)
								{
									FVector NewLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnPoint->GetCenterOfMass(), SpawnPoint->GetScaledBoxExtent());

									SpawnedEnemy = Cast<AEnemy>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, SpawnInfo.EnemyClass, FTransform{ NewLocation },
										ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));
									if (SpawnedEnemy != nullptr)
									{
										SpawnedEnemy->SetEnemyLevel(SpawnInfo.EnemyLevel);
										SpawnedEnemy->KillEnemyEventDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDead);
										SpawnedEnemy = Cast<AEnemy>(UGameplayStatics::FinishSpawningActor(SpawnedEnemy, FTransform{ NewLocation }));
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AEnemySpawner::SpawnEnemys(int WaveLevel)
{
	if (EnemySpawnTable != nullptr)
	{
		FString RowName = FString::Printf(TEXT("Wave%d"), WaveLevel);
		CurrentWaveSpawnDataRow = EnemySpawnTable->FindRow<FEnemySpawnTable>(FName(*RowName), "");

		PreviousTime = -1;
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, 31.0f, false);

		if (CurrentWaveSpawnDataRow != nullptr && WaveManager != nullptr)
		{
			for (const auto& [Time, SpawnTimeInfo] : CurrentWaveSpawnDataRow->SpawnEnemys)
			{
				for (const auto& SpawnInfo : SpawnTimeInfo.SpawnEnemys)
				{
					WaveManager->IncreaseEnemyCount(SpawnInfo.NumOfEnemys);
				}
			}
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawnTable is nullptr"));
	}
}

void AEnemySpawner::OnEnemyDead()
{
	if (WaveManager != nullptr)
	{
		WaveManager->DecreaseEnemyCount(1);
	}
}