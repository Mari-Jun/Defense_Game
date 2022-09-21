// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "CharacterUpgradeWidget.generated.h"

USTRUCT(BlueprintType)
struct FCharacterUpgradeTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 AttackUpgradeFee;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 DefenseUpgradeFee;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 HPUpgradeFee;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 ShieldUpgradeFee;
};

class ABasePlayerController;
class UUpgradeStatusWidget;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UCharacterUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
private:
	void InitializeAtatckWidget();
	UFUNCTION()
	void OnUpgradeAttack();
	
private:
	ABasePlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUpgradeStatusWidget* AttackWidget;

public:
	void SetPlayerController(ABasePlayerController* Controller);
};
