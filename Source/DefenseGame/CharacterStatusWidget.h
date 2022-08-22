// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatusWidget.generated.h"

class ABaseCharacter;
class UCharacterProgressBarWidget;

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

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* BaseCharacter;

public:
	void SetBaseCharacter(ABaseCharacter* Character);
};
