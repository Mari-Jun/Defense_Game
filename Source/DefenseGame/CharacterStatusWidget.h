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

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* BaseCharacter;

public:
	void SetBaseCharacter(ABaseCharacter* Character);

	UCharacterSkillTimeWidget* GetAbilityWidget(int32 AbilityIndex) const;
};
