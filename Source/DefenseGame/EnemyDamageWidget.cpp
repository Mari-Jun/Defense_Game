// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDamageWidget.h"

#include "Components/TextBlock.h"

void UEnemyDamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	DamageText->SetColorAndOpacity(FLinearColor{ 1.0f, 0.0f, 0.0f, FMath::Clamp(TextAlpha, 0.0f, 1.0f) });
	TextAlpha -= InDeltaTime;
}

void UEnemyDamageWidget::SetDamageText(float Damage)
{
	int32 IntDamage = FMath::RoundToInt32(Damage);
	DamageText->SetText(FText::FromString(FString::FromInt(IntDamage)));
}

void UEnemyDamageWidget::SetTextAlpha(float Alpha)
{
	TextAlpha = Alpha;
}
