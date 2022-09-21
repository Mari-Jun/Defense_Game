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

	if (PlayerController == nullptr)
	{
		PlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
		InitializeAtatckWidget();
	}
}

void UCharacterUpgradeWidget::InitializeAtatckWidget()
{
	if (PlayerController != nullptr && AttackWidget != nullptr)
	{
		int32 CurrentLevel = AttackWidget->GetCurrentLevel();
		UDataTable* DataTable = PlayerController->GetUpgradeStatusDataTable();

		FString RowName = FString::Printf(TEXT("Level%d"), CurrentLevel);
		FCharacterUpgradeTable* UpgradeTable = DataTable->FindRow<FCharacterUpgradeTable>(FName(RowName), "");

		if (UpgradeTable != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, TEXT("Hello World"));
			AttackWidget->Upgrade(CurrentLevel, UpgradeTable->AttackUpgradeFee);
		}
	}
}

void UCharacterUpgradeWidget::OnUpgradeAttack()
{
	if (PlayerController != nullptr && AttackWidget != nullptr)
	{	
		int32 CurrentLevel = AttackWidget->GetCurrentLevel();
		UDataTable* DataTable = PlayerController->GetUpgradeStatusDataTable();

		ABaseCharacter* BaseCharacter = PlayerController->GetBaseCharacter();
		if (BaseCharacter != nullptr)
		{
			int32 Coin = BaseCharacter->GetCoin();
			if (Coin >= AttackWidget->GetCurrentFee())
			{
				BaseCharacter->ChangeCoin(-AttackWidget->GetCurrentFee());
				BaseCharacter->ChangeCombatStatusAttack(5);

				FString RowName = FString::Printf(TEXT("Level%d"), CurrentLevel + 1);
				FCharacterUpgradeTable* UpgradeTable = DataTable->FindRow<FCharacterUpgradeTable>(FName(RowName), "");
				if (UpgradeTable != nullptr)
				{
					AttackWidget->Upgrade(CurrentLevel + 1, UpgradeTable->AttackUpgradeFee);
				}
				else
				{
					AttackWidget->UpgradeComplete();
				}
			}
		}
	}
}

void UCharacterUpgradeWidget::SetPlayerController(ABasePlayerController* Controller)
{
	PlayerController = Controller;
}
