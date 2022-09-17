// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterRespawnWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterRespawnWidget::SetRespawnTime(float Time, float MaxTime)
{
	RespawnBar->SetPercent((MaxTime - Time) / MaxTime);
	RespawnTime->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt32(MaxTime - Time))));
}
