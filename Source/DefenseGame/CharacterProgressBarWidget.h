// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterProgressBarWidget.generated.h"

class UOverlay;
class UCanvasPanelSlot;
class UImage;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UCharacterProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

public:
	UFUNCTION()
	void OnChangeValue(float CurrentValue, float MaxValue);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* BaseOverlay;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* ProgressBar;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* BackgroundImage;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* EffectImage;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* ValueText;

	UPROPERTY(EditAnywhere)
	FLinearColor ProgressTintColor;

public:
	UProgressBar* GetProgressBar() const { return ProgressBar; }
	UTextBlock* GetValueText() const { return ValueText; }
};
