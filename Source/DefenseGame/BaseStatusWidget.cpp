// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStatusWidget.h"

#include "Components/TextBlock.h"

void UBaseStatusWidget::NativePreConstruct()
{
}

void UBaseStatusWidget::OnChangeHP(float CurrentHP, float MaxHP)
{
	int32 IntCurrentHP = FMath::RoundToInt32(CurrentHP);
	int32 IntmMaxHP = FMath::RoundToInt32(MaxHP);

	HPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), IntCurrentHP, IntmMaxHP)));
}

void UBaseStatusWidget::OnChangeShield(float CurrentShield, float MaxShield)
{
	int32 IntCurrentShield = FMath::RoundToInt32(CurrentShield);
	int32 IntmMaxShield = FMath::RoundToInt32(MaxShield);

	ShieldText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), IntCurrentShield, IntmMaxShield)));
}
