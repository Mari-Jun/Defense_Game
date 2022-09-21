// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeStatusWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUpgradeStatusWidget::Upgrade(int32 NewLevel, int32 NewFee)
{
	CurrentUpgradeLevel = NewLevel;
	CurrentFee = NewFee;
	UpgradeLevelText->SetText(FText::FromString("Level " + FString::FromInt(NewLevel)));
	UpgradeFeeText->SetText(FText::FromString(FString::FromInt(NewFee)));
}

void UUpgradeStatusWidget::UpgradeComplete()
{
	UpgradeLevelText->SetText(FText::FromString("Complete"));
	UpgradeFeeText->SetText(FText::FromString("XXX"));
	UpgradeButton->SetIsEnabled(false);
}

