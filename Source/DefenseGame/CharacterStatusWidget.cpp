// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusWidget.h"
#include "CharacterProgressBarWidget.h"
#include "CharacterSkillTimeWidget.h"
#include "BaseCharacter.h"

#include "Internationalization/Text.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"

void UCharacterStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (AbilityQWidget != nullptr) { AbilityQWidget->SetKeyText("Q"); }
	if (AbilityEWidget != nullptr) { AbilityEWidget->SetKeyText("E"); }
	if (AbilityRWidget != nullptr) { AbilityRWidget->SetKeyText("R"); }
	if (AbilityRMBWidget != nullptr) { AbilityRMBWidget->SetKeyText("RMB"); }
}

void UCharacterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BaseCharacter != nullptr)
	{
		if (HPProgressBar != nullptr)
		{
			BaseCharacter->ChangeHPDelegate.AddDynamic(this, &UCharacterStatusWidget::OnChangeHP);
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

	PostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
}

void UCharacterStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UCharacterStatusWidget::OnChangeHP(float CurrentHP, float MaxHP)
{
	HPProgressBar->OnChangeHP(CurrentHP, MaxHP);

	if (PostProcessVolume != nullptr)
	{
		if (CurrentHP <= MaxHP * 0.2f)
		{
			PostProcessVolume->Settings.WeightedBlendables.Array[0].Weight = 1.0f;
		}
		else
		{
			PostProcessVolume->Settings.WeightedBlendables.Array[0].Weight = 0.0f;
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
