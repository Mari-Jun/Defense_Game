// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "EnemySpawner.h"
#include "WaveInfoWidget.h"
#include "BasePlayerController.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

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
			WaveInfoWidget->AddToViewport(1);
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
			EnemySpawner->SetWaveManager(this);
			EnemySpawners.Add(EnemySpawner);
		}
	}
}

void AWaveManager::WaitNextWave()
{
	++CurrentWave;
	GetWaveSoundData();
	PlayWaitWaveSound();

	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::StartWave, WaveWaitTime);
	if (WaveInfoWidget != nullptr)
	{
		WaveInfoWidget->SetNextWaveWidgetVisibility(true);
		WaveInfoWidget->SetCurrentWaveWidgetVisibility(false);
		WaveInfoWidget->SetUseCharacterUpgrade(true);
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController != nullptr)
		{
			PlayerController->SetCaneUseUpgradeWidget(true);
		}
	}
}

void AWaveManager::StartWave()
{
	PlayBattleSound();

	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	for (const auto& EnemySpanwer : EnemySpawners)
	{
		EnemySpanwer->SpawnEnemys(CurrentWave);
	}

	if (WaveInfoWidget != nullptr)
	{
		WaveInfoWidget->SetCurrentWaveText(CurrentWave);
		WaveInfoWidget->SetNextWaveWidgetVisibility(false);
		WaveInfoWidget->SetCurrentWaveWidgetVisibility(true);
		WaveInfoWidget->SetUseCharacterUpgrade(false);
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController != nullptr)
		{
			PlayerController->SetCaneUseUpgradeWidget(false);
		}
	}
}

void AWaveManager::KilledAllSpawnedEnemy()
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

void AWaveManager::GetWaveSoundData()
{
	if (WaveSoundTable != nullptr)
	{
		FString RowName = FString::Printf(TEXT("Wave%d"), CurrentWave);
		CurrentWaveSoundData = WaveSoundTable->FindRow<FWaveSoundTable>(FName(*RowName), "");
	}
}

void AWaveManager::PlayWaitWaveSound()
{
	if (CurrentWaveSoundData != nullptr && CurrentWaveSoundData->WaitWaveSound != nullptr)
	{
		if (CurrentBGM != nullptr)
			CurrentBGM->Stop();
		CurrentBGM = UGameplayStatics::SpawnSound2D(this, CurrentWaveSoundData->WaitWaveSound);
		CurrentBGM->Play();
	}
}

void AWaveManager::PlayBattleSound()
{
	if (CurrentWaveSoundData != nullptr && CurrentWaveSoundData->BattleSound != nullptr)
	{
		if (CurrentBGM != nullptr)
			CurrentBGM->Stop();
		CurrentBGM = UGameplayStatics::SpawnSound2D(this, CurrentWaveSoundData->BattleSound);
		CurrentBGM->Play();
	}
}

void AWaveManager::IncreaseEnemyCount(int32 Value)
{
	WaveNumOfEnemys += Value;
	WaveInfoWidget->SetCurrentWaveRemainingEnemy(WaveNumOfEnemys);
}

void AWaveManager::DecreaseEnemyCount(int32 Value)
{
	WaveNumOfEnemys -= Value;
	WaveInfoWidget->SetCurrentWaveRemainingEnemy(WaveNumOfEnemys);
	if (WaveNumOfEnemys == 0)
	{
		KilledAllSpawnedEnemy();
	}
}

