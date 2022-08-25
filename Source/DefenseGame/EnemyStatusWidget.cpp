// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusWidget.h"

#include "Components/ProgressBar.h"

void UEnemyStatusWidget::NativePreConstruct()
{
}

void UEnemyStatusWidget::OnChangeHP(float CurrentHP, float MaxHP)
{
	HealthBar->SetPercent(CurrentHP / MaxHP);
}