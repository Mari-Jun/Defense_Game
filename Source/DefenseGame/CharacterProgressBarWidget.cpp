// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProgressBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCharacterProgressBarWidget::NativePreConstruct()
{
}

void UCharacterProgressBarWidget::OnChangeHP(int32 CurrentHP, int32 MaxHP)
{
	ProgressBar->SetPercent(static_cast<float>(CurrentHP) / MaxHP);
	ValueText->SetText(FText::FromString(FString::FromInt(CurrentHP)));
}
