// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterUpgradeWidget.h"
#include "UpgradeStatusWidget.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"

#include "Components/Button.h"

void UCharacterUpgradeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AttackWidget->GetUpgradeButton()->OnClicked.AddDynamic(this, &UCharacterUpgradeWidget::OnUpgradeAttack);
	DefenseWidget->GetUpgradeButton()->OnClicked.AddDynamic(this, &UCharacterUpgradeWidget::OnUpgradeDefense);
	CriticalWidget->GetUpgradeButton()->OnClicked.AddDynamic(this, &UCharacterUpgradeWidget::OnUpgradeCritical);
	HPWidget->GetUpgradeButton()->OnClicked.AddDynamic(this, &UCharacterUpgradeWidget::OnUpgradeHP);
	ShieldWidget->GetUpgradeButton()->OnClicked.AddDynamic(this, &UCharacterUpgradeWidget::OnUpgradeShield);

	if (PlayerController == nullptr)
	{
		PlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
	}

	InitializeUpgradeWidget(AttackWidget, UpgradeAttackDataTable);
	InitializeUpgradeWidget(DefenseWidget, UpgradeDefenseDataTable);
	InitializeUpgradeWidget(CriticalWidget, UpgradeCriticalDataTable);
	InitializeUpgradeWidget(HPWidget, UpgradeHPDataTable);
	InitializeUpgradeWidget(ShieldWidget, UpgradeShieldDataTable);
}

void UCharacterUpgradeWidget::InitializeUpgradeWidget(UUpgradeStatusWidget * UpgradeWidget, UDataTable* DataTable) const
{
	if (UpgradeWidget != nullptr && DataTable != nullptr)
	{
		int32 CurrentLevel = UpgradeWidget->GetUpgradeLevel();

		FString RowName = FString::Printf(TEXT("Level%d"), CurrentLevel);
		FCharacterUpgradeTable* UpgradeTable = DataTable->FindRow<FCharacterUpgradeTable>(FName(RowName), "");

		if (UpgradeTable != nullptr)
		{
			UpgradeWidget->Upgrade(CurrentLevel, UpgradeTable->Fee, UpgradeTable->Value);
		}
	}
}

void UCharacterUpgradeWidget::UpgradeStatus(UUpgradeStatusWidget * UpgradeWidget, UDataTable* DataTable,
	std::function<void(ABaseCharacter* BaseCharacter, float Value)> ExecFunction) const
{
	if (PlayerController != nullptr && UpgradeWidget != nullptr && DataTable != nullptr)
	{
		int32 CurrentLevel = UpgradeWidget->GetUpgradeLevel();

		ABaseCharacter* BaseCharacter = PlayerController->GetBaseCharacter();
		if (BaseCharacter != nullptr)
		{
			int32 Coin = BaseCharacter->GetCoin();
			if (Coin >= UpgradeWidget->GetUpgradeFee())
			{
				BaseCharacter->ChangeCoin(-UpgradeWidget->GetUpgradeFee());
				ExecFunction(BaseCharacter, UpgradeWidget->GetUpgradeValue());

				FString RowName = FString::Printf(TEXT("Level%d"), CurrentLevel + 1);
				FCharacterUpgradeTable* UpgradeTable = DataTable->FindRow<FCharacterUpgradeTable>(FName(RowName), "");
				if (UpgradeTable != nullptr)
				{
					UpgradeWidget->Upgrade(CurrentLevel + 1, UpgradeTable->Fee, UpgradeTable->Value);
				}
				else
				{
					UpgradeWidget->UpgradeComplete();
				}
			}
		}
	}
}

void UCharacterUpgradeWidget::OnUpgradeAttack()
{
	UpgradeStatus(AttackWidget, UpgradeAttackDataTable, 
		[](ABaseCharacter* BaseCharacter, float Value) { BaseCharacter->ChangeCombatStatusAttack(Value); });
}

void UCharacterUpgradeWidget::OnUpgradeDefense()
{
	UpgradeStatus(DefenseWidget, UpgradeDefenseDataTable,
		[](ABaseCharacter* BaseCharacter, float Value) { BaseCharacter->ChangeCombatStatusDefense(Value); });
}

void UCharacterUpgradeWidget::OnUpgradeCritical()
{
	UpgradeStatus(CriticalWidget, UpgradeCriticalDataTable,
		[](ABaseCharacter* BaseCharacter, float Value) { BaseCharacter->ChangeCombatStatusCritical(Value / 100.0f); });
}

void UCharacterUpgradeWidget::OnUpgradeHP()
{
	UpgradeStatus(HPWidget, UpgradeHPDataTable, 
		[](ABaseCharacter* BaseCharacter, float Value) {
			BaseCharacter->ChangeCombatStatusMaxHP(Value); 
			BaseCharacter->ChangeCombatStatusCurrentHP(Value); 
		});
}

void UCharacterUpgradeWidget::OnUpgradeShield()
{
	UpgradeStatus(ShieldWidget, UpgradeShieldDataTable,
		[](ABaseCharacter* BaseCharacter, float Value) {
			BaseCharacter->ChangeCombatStatusMaxShield(Value); 
			BaseCharacter->ChangeCombatStatusCurrentShield(Value); 
		});
}

void UCharacterUpgradeWidget::SetPlayerController(ABasePlayerController* Controller)
{
	PlayerController = Controller;
}
