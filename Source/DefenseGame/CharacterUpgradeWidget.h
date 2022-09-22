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
	int32 Fee;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	float Value;
};

class ABaseCharacter;
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
	void InitializeUpgradeWidget(UUpgradeStatusWidget* UpgradeWidget, UDataTable* DataTable) const;
	void UpgradeStatus(UUpgradeStatusWidget* UpgradeWidget, UDataTable* DataTable, 
		std::function<void(ABaseCharacter* BaseCharacter, float Value)> ExecFunction) const;
	UFUNCTION()
	void OnUpgradeAttack();
	UFUNCTION()
	void OnUpgradeDefense();
	UFUNCTION()
	void OnUpgradeCritical();
	UFUNCTION()
	void OnUpgradeHP();
	UFUNCTION()
	void OnUpgradeShield();
	
private:
	ABasePlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUpgradeStatusWidget* AttackWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUpgradeStatusWidget* DefenseWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUpgradeStatusWidget* CriticalWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUpgradeStatusWidget* HPWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUpgradeStatusWidget* ShieldWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* UpgradeAttackDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* UpgradeDefenseDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* UpgradeCriticalDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* UpgradeHPDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* UpgradeShieldDataTable;

public:
	void SetPlayerController(ABasePlayerController* Controller);
};
