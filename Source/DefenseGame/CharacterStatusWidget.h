// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatusWidget.generated.h"

class ABaseCharacter;
class UCharacterProgressBarWidget;
class UCharacterSkillTimeWidget;

class UCanvasPanel;
class UCanvasPanelSlot;
class UImage;
class UProgressBar;
class UTextLayoutWidget;
class APostProcessVolume;
/**
 * 
 */

UCLASS()
class DEFENSEGAME_API UCharacterStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void OnChangeHP(float CurrentHP, float MaxHP);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterProgressBarWidget* HPProgressBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityQWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityEWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityRWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityRMBWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* BaseCharacter;

	APostProcessVolume* PostProcessVolume;

public:
	void SetBaseCharacter(ABaseCharacter* Character);

	UCharacterSkillTimeWidget* GetAbilityWidget(int32 AbilityIndex) const;
};
