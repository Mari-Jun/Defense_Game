// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusWidget.h"
#include "CharacterProgressBarWidget.h"
#include "CharacterSkillTimeWidget.h"
#include "CharacterRespawnWidget.h"
#include "BaseCharacter.h"

#include "Internationalization/Text.h"
#include "Components/Image.h"
#include "Curves/CurveLinearColor.h"

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

		if (HitImage != nullptr)
		{
			BaseCharacter->HitShakeCameraDelegate.AddDynamic(this, &UCharacterStatusWidget::OnHitReaction);
		}
	}

	SetRespawnWidgetVisibility(false);

	PostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));
}

void UCharacterStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (HitImageColorTimer.IsValid())
	{
		const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(HitImageColorTimer);

		FLinearColor ImageColor = HitImageLinearColorCurve->GetLinearColorValue(ElapsedTime);
		HitImage->SetBrushTintColor(ImageColor);
	}
}

void UCharacterStatusWidget::OnChangeHP(float CurrentHP, float MaxHP)
{
	HPProgressBar->OnChangeValue(CurrentHP, MaxHP);

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

void UCharacterStatusWidget::OnChangeShield(float CurrentShield, float MaxShield)
{
	ShieldProgressBar->OnChangeValue(CurrentShield, MaxShield);
}

void UCharacterStatusWidget::OnHitReaction()
{
	GetWorld()->GetTimerManager().SetTimer(HitImageColorTimer, 0.25f, false);
}

void UCharacterStatusWidget::SetRespawnWidgetVisibility(bool IsVisible)
{
	IsVisible ? RespawnWidget->SetVisibility(ESlateVisibility::Visible) : RespawnWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterStatusWidget::SetRespawnTime(float Time, float MaxTime)
{
	RespawnWidget->SetRespawnTime(Time, MaxTime);
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
