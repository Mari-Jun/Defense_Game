// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameResultWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowWinResult();
	void ShowDefeatResult();
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void QuitGame();

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* WinText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* WinImage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* DefeatText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* DefeatImage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ExitButton;
};
