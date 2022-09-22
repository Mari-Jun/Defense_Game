// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeStatusWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UUpgradeStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Upgrade(int32 NewLevel, int32 NewFee, float NewValue);
	void UpgradeComplete();
	
private:
	int32 UpgradeLevel = 1;
	int32 UpgradeFee = 0;
	float UpgradeValue = 0;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeLevelText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeFeeText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeValueText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* UpgradeButton;

public:
	int32 GetUpgradeLevel() const { return UpgradeLevel; }
	int32 GetUpgradeFee() const { return UpgradeFee; }
	float GetUpgradeValue() const { return UpgradeValue; }

	UButton* GetUpgradeButton() const { return UpgradeButton; }
};
