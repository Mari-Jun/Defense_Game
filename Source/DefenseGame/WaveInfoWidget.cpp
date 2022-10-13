// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveInfoWidget.h"

#include "Components/TextBlock.h"

void UWaveInfoWidget::SetNextWaveWidgetVisibility(bool Visible)
{
	ESlateVisibility NewVisibility;
	if (Visible)
	{
		NewVisibility = ESlateVisibility::Visible;
	}
	else
	{
		NewVisibility = ESlateVisibility::Hidden;
	}

	NextWaveRemainingTimeText->SetVisibility(NewVisibility);
}

void UWaveInfoWidget::SetCurrentWaveWidgetVisibility(bool Visible)
{
	ESlateVisibility NewVisibility;
	if (Visible)
	{
		NewVisibility = ESlateVisibility::Visible;
	}
	else
	{
		NewVisibility = ESlateVisibility::Hidden;
	}

	CurrentWaveRemainingEnemyText->SetVisibility(NewVisibility);
}

void UWaveInfoWidget::SetNextWaveRemainingTime(float RemainingTime)
{
	FText Text = FText::FromString(L"다음 웨이브까지 : " + FString::FromInt(FMath::CeilToInt(RemainingTime)));
	NextWaveRemainingTimeText->SetText(Text);
}

void UWaveInfoWidget::SetCurrentWaveText(int32 CurrentWave)
{
	FText Text = FText::FromString(L"현재 웨이브 : " + FString::FromInt(CurrentWave));
	CurrentWaveText->SetText(Text);
}

void UWaveInfoWidget::SetCurrentWaveRemainingEnemy(int32 EnemyNums)
{
	FText Text = FText::FromString(L"남은 적의 수 : " + FString::FromInt(EnemyNums));
	CurrentWaveRemainingEnemyText->SetText(Text);
}

void UWaveInfoWidget::SetUseCharacterUpgrade(bool bUse)
{
	FText Text;
	if (bUse)
	{
		Text = FText::FromString(L"상점 이용이 가능합니다.");
	}
	else
	{
		Text = FText::FromString(L"상점 이용이 불가능합니다.");
	}

	UpgradeInfoText->SetText(Text);
}
