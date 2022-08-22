// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusWidget.h"
#include "CharacterProgressBarWidget.h"
#include "BaseCharacter.h"

void UCharacterStatusWidget::NativePreConstruct()
{
}

void UCharacterStatusWidget::NativeConstruct()
{
	if (BaseCharacter != nullptr)
	{
		if (HPProgressBar != nullptr)
		{
			BaseCharacter->ChangeHPDelegate.AddDynamic(HPProgressBar, &UCharacterProgressBarWidget::OnChangeHP);
		}
	}
}

void UCharacterStatusWidget::SetBaseCharacter(ABaseCharacter* Character)
{
	BaseCharacter = Character;
}
