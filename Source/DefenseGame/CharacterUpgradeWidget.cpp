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
	if (AttackWidget != nullptr)
	{
		int32 CurrentLevel = AttackWidget->GetCurrentLevel();

		FString RowName = FString::Printf(TEXT("Level%d"), CurrentLevel);
		FCharacterUpgradeTable* UpgradeTable = UpgradeAttackDataTable->FindRow<FCharacterUpgradeTable>(FName(RowName), "");

		if (UpgradeTable != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, TEXT("Hello World"));
			AttackWidget->Upgrade(CurrentLevel, UpgradeTable->UpgradeFee);
		}
	}
}

void UCharacterUpgradeWidget::OnUpgradeAttack()
{
	if (PlayerController != nullptr && AttackWidget != nullptr)
	{	
		int32 CurrentLevel = AttackWidget->GetCurrentLevel();

		ABaseCharacter* BaseCharacter = PlayerController->GetBaseCharacter();
		if (BaseCharacter != nullptr)
		{
			int32 Coin = BaseCharacter->GetCoin();
			if (Coin >= AttackWidget->GetCurrentFee())
			{
				BaseCharacter->ChangeCoin(-AttackWidget->GetCurrentFee());
				BaseCharacter->ChangeCombatStatusAttack(5);

				FString RowName = FString::Printf(TEXT("Level%d"), CurrentLevel + 1);
				FCharacterUpgradeTable* UpgradeTable = UpgradeAttackDataTable->FindRow<FCharacterUpgradeTable>(FName(RowName), "");
				if (UpgradeTable != nullptr)
				{
					AttackWidget->Upgrade(CurrentLevel + 1, UpgradeTable->UpgradeFee);
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
