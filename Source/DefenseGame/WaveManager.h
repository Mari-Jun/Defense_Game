// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WaveManager.generated.h"

class AEnemySpawner;
class UWaveInfoWidget;
class UAudioComponent;
class USoundCue;

USTRUCT(BlueprintType)
struct FWaveSoundTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* WaitWaveSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* BattleSound;
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
	void KilledAllSpawnedEnemy();


	//Sound
	void GetWaveSoundData();
	void PlayWaitWaveSound();
	void PlayBattleSound();
	

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave", meta = (AllowPrivateAccess = "true"))
	int32 WaveWaitTime = 10;

	int32 CurrentWave = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave", meta = (AllowPrivateAccess = "true"))
	int32 MaxWave = 1;
	FTimerHandle WaveTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWaveInfoWidget> WaveInfoWidgetClass;
	UPROPERTY()
	UWaveInfoWidget* WaveInfoWidget;

	int32 WaveNumOfEnemys = 0;

	TArray<AEnemySpawner*> EnemySpawners;

	//Sound
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	UDataTable* WaveSoundTable;
	FWaveSoundTable* CurrentWaveSoundData;
	UPROPERTY()
	UAudioComponent* CurrentBGM;

public:
	void IncreaseEnemyCount(int32 Value);
	void DecreaseEnemyCount(int32 Value);
};
