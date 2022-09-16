// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDamageWidget.h"

#include "Components/TextBlock.h"

void UEnemyDamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (HPDamageText->GetVisibility() == ESlateVisibility::Visible)
	{
		HPDamageText->SetRenderOpacity(TextAlpha);
	}
	if (ShieldDamageText->GetVisibility() == ESlateVisibility::Visible)
	{
		ShieldDamageText->SetRenderOpacity(TextAlpha);
	}
	if (HPCriticalDamageText->GetVisibility() == ESlateVisibility::Visible)
	{
		HPCriticalDamageText->SetRenderOpacity(TextAlpha);
	}
	if (ShieldCriticalDamageText->GetVisibility() == ESlateVisibility::Visible)
	{
		ShieldCriticalDamageText->SetRenderOpacity(TextAlpha);
	}
	TextAlpha -= InDeltaTime;
}

void UEnemyDamageWidget::SetDamageText(float HPDamage, float ShieldDamage, bool IsCritical)
{
	HPDamageText->SetVisibility(ESlateVisibility::Hidden);
	HPCriticalDamageText->SetVisibility(ESlateVisibility::Hidden);
	ShieldDamageText->SetVisibility(ESlateVisibility::Hidden);
	ShieldCriticalDamageText->SetVisibility(ESlateVisibility::Hidden);

	static auto SetDamageText = [](UTextBlock* DamageText, float Damage) {
		DamageText->SetVisibility(ESlateVisibility::Visible);
		int32 IntDamage = FMath::RoundToInt32(Damage);
		FText TextDamage = FText::FromString(FString::FromInt(IntDamage));
		DamageText->SetText(TextDamage);
	};

	if (HPDamage > 0.0f)
	{
		IsCritical ? SetDamageText(HPCriticalDamageText, HPDamage) : SetDamageText(HPDamageText, HPDamage);
	}

	if (ShieldDamage > 0.0f)
	{
		IsCritical ? SetDamageText(ShieldCriticalDamageText, ShieldDamage) : SetDamageText(ShieldDamageText, ShieldDamage);
	}
}

void UEnemyDamageWidget::SetTextAlpha(float Alpha)
{
	TextAlpha = Alpha;
}
