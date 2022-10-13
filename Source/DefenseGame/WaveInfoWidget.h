// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveInfoWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UWaveInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* NextWaveRemainingTimeText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* CurrentWaveRemainingEnemyText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* CurrentWaveText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeKeyText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeInfoText;


public:
	void SetNextWaveWidgetVisibility(bool Visible);
	void SetCurrentWaveWidgetVisibility(bool Visible);

	void SetNextWaveRemainingTime(float RemainingTime);
	void SetCurrentWaveText(int32 CurrentWave);
	void SetCurrentWaveRemainingEnemy(int32 EnemyNums);
	void SetUseCharacterUpgrade(bool bUse);
};
