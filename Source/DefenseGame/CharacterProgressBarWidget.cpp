// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProgressBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterProgressBarWidget::NativePreConstruct()
{
	ProgressBar->SetFillColorAndOpacity(ProgressTintColor);
}

void UCharacterProgressBarWidget::OnChangeValue(float CurrentValue, float MaxValue)
{
	ProgressBar->SetPercent(CurrentValue / MaxValue);
	ValueText->SetText(FText::FromString(FString::FromInt(FMath::FloorToInt(CurrentValue))));
}
