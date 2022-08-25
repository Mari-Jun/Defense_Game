// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProgressBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterProgressBarWidget::NativePreConstruct()
{
}

void UCharacterProgressBarWidget::OnChangeHP(float CurrentHP, float MaxHP)
{
	ProgressBar->SetPercent(CurrentHP / MaxHP);
	ValueText->SetText(FText::FromString(FString::FromInt(FMath::FloorToInt(CurrentHP))));
}
