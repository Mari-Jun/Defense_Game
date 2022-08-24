// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusWidget.h"
#include "CharacterProgressBarWidget.h"
#include "CharacterSkillTimeWidget.h"
#include "BaseCharacter.h"

#include "Internationalization/Text.h"

void UCharacterStatusWidget::NativePreConstruct()
{
	if (AbilityQWidget != nullptr) { AbilityQWidget->SetKeyText("Q"); }
	if (AbilityEWidget != nullptr) { AbilityEWidget->SetKeyText("E"); }
	if (AbilityRWidget != nullptr) { AbilityRWidget->SetKeyText("R"); }
	if (AbilityRMBWidget != nullptr) { AbilityRMBWidget->SetKeyText("RMB"); }
}

void UCharacterStatusWidget::NativeConstruct()
{
	if (BaseCharacter != nullptr)
	{
		if (HPProgressBar != nullptr)
		{
			BaseCharacter->ChangeHPDelegate.AddDynamic(HPProgressBar, &UCharacterProgressBarWidget::OnChangeHP);
		}

		if (AbilityQWidget != nullptr)
		{
			BaseCharacter->ChangeAbilityTimeDelegate[0].AddDynamic(AbilityQWidget, &UCharacterSkillTimeWidget::OnChangeCooldownState);
		}
		if (AbilityEWidget != nullptr)
		{
			BaseCharacter->ChangeAbilityTimeDelegate[1].AddDynamic(AbilityEWidget, &UCharacterSkillTimeWidget::OnChangeCooldownState);
		}
		if (AbilityRWidget != nullptr)
		{
			BaseCharacter->ChangeAbilityTimeDelegate[2].AddDynamic(AbilityRWidget, &UCharacterSkillTimeWidget::OnChangeCooldownState);
		}
		if (AbilityRMBWidget != nullptr)
		{
			BaseCharacter->ChangeAbilityTimeDelegate[3].AddDynamic(AbilityRMBWidget, &UCharacterSkillTimeWidget::OnChangeCooldownState);
		}
	}
}

void UCharacterStatusWidget::SetBaseCharacter(ABaseCharacter* Character)
{
	BaseCharacter = Character;
}

UCharacterSkillTimeWidget* UCharacterStatusWidget::GetAbilityWidget(int32 AbilityIndex) const
{
	switch (AbilityIndex)
	{
	case 0: return AbilityQWidget;
	case 1: return AbilityEWidget;
	case 2: return AbilityRWidget;
	case 3: return AbilityRMBWidget;
	default: return nullptr;
	}
}
