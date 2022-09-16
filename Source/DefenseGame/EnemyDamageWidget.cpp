// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDamageWidget.h"

#include "Components/TextBlock.h"

void UEnemyDamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (HPDamageText->GetVisibility() == ESlateVisibility::Visible)
	{
		HPDamageText->SetColorAndOpacity(FLinearColor{ 1.0f, 0.0f, 0.0f, FMath::Clamp(TextAlpha, 0.0f, 1.0f) });
	}
	else if (ShieldDamageText->GetVisibility() == ESlateVisibility::Visible)
	{
		ShieldDamageText->SetColorAndOpacity(FLinearColor{ 1.0f, 1.0f, 1.0f, FMath::Clamp(TextAlpha, 0.0f, 1.0f) });
	}
	TextAlpha -= InDeltaTime;
}

void UEnemyDamageWidget::SetDamageText(float HPDamage, float ShieldDamage, bool IsCritical)
{
	ShieldDamageText->SetVisibility(ESlateVisibility::Hidden);
	HPDamageText->SetVisibility(ESlateVisibility::Hidden);

	static auto SetDamageText = [](UTextBlock* DamageText, float Damage) {
		DamageText->SetVisibility(ESlateVisibility::Visible);
		int32 IntDamage = FMath::RoundToInt32(Damage);
		FText TextDamage = FText::FromString(FString::FromInt(IntDamage));
		DamageText->SetText(TextDamage);
	};

	if (HPDamage > 0.0f)
	{
		SetDamageText(HPDamageText, HPDamage);
	}

	if (ShieldDamage > 0.0f)
	{
		SetDamageText(ShieldDamageText, ShieldDamage);
	}
}

void UEnemyDamageWidget::SetTextAlpha(float Alpha)
{
	TextAlpha = Alpha;
}
