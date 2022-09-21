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
	void Upgrade(int32 NewLevel, int32 NewFee);
	void UpgradeComplete();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	int32 MaxUpgradeLevel = 20;
	int32 CurrentUpgradeLevel = 1;
	int32 CurrentFee = 0;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeLevelText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* UpgradeFeeText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* UpgradeButton;

public:
	int32 GetMaxLevel() const { return MaxUpgradeLevel; }
	int32 GetCurrentLevel() const { return CurrentUpgradeLevel; }
	int32 GetCurrentFee() const { return CurrentFee; }

	UButton* GetUpgradeButton() const { return UpgradeButton; }
};
