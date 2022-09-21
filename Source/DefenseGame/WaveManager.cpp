// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "EnemySpawner.h"
#include "WaveInfoWidget.h"
#include "BasePlayerController.h"

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

	if (WaveInfoWidgetClass != nullptr)
	{
		WaveInfoWidget = CreateWidget<UWaveInfoWidget>(GetWorld()->GetFirstPlayerController(), WaveInfoWidgetClass);
		if (WaveInfoWidget != nullptr)
		{
			WaveInfoWidget->AddToPlayerScreen();
		}
	}

	GetEnemySpawner();

	WaitNextWave();
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaveTimerHandle.IsValid() && WaveInfoWidget != nullptr)
	{
		float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(WaveTimerHandle);
		WaveInfoWidget->SetNextWaveRemainingTime(WaveWaitTime - ElapsedTime);
	}
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
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::StartWave, WaveWaitTime);
	if (WaveInfoWidget != nullptr)
	{
		WaveInfoWidget->SetNextWaveWidgetVisibility(true);
	}
}

void AWaveManager::StartWave()
{
	++CurrentWave;
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	for (auto& [EnemySpanwer, EmptyEnemy] : EnemySpawners)
	{
		EnemySpanwer->SpawnEnemys(CurrentWave);
		EmptyEnemy = ESpawnerEmptyState::ENotEmpty;
	}
	if (WaveInfoWidget != nullptr)
	{
		WaveInfoWidget->SetCurrentWaveText(CurrentWave);
		WaveInfoWidget->SetNextWaveWidgetVisibility(false);
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
			ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
			if (PlayerController != nullptr)
			{
				PlayerController->ShowGameResult(true);
			}
		}
		else
		{
			WaitNextWave();
		}
	}
}

