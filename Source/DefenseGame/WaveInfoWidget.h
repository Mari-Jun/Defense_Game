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
	UTextBlock* CurrentWaveText;

public:
	void SetNextWaveWidgetVisibility(bool Visible);

	void SetNextWaveRemainingTime(float RemainingTime);
	void SetCurrentWaveText(int32 CurrentWave);
};
