// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSkillTimeWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCharacterSkillTimeWidget::NativePreConstruct()
{
	SetCooldownVisibility(false);
}

void UCharacterSkillTimeWidget::OnChangeCooldownState(float Time)
{
	TimeText->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt32(Time))));
}

void UCharacterSkillTimeWidget::SetKeyText(FString text)
{
	KeyText->SetText(FText::FromString(text));
}

void UCharacterSkillTimeWidget::SetCooldownVisibility(bool visible)
{
	if (visible)
	{
		CooldownImage->SetVisibility(ESlateVisibility::Visible);
		TimeText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CooldownImage->SetVisibility(ESlateVisibility::Hidden);
		TimeText->SetVisibility(ESlateVisibility::Hidden);
	}
}
