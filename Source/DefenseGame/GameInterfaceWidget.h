// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInterfaceWidget.generated.h"

class UCanvasPanel;
class UCanvasPanelSlot;
class UButton;
class UImage;
class UProgressBar;
class UTextLayoutWidget;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UGameInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	
private:
	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void QuitGame();

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ResumeButton;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ExitButton;
};
