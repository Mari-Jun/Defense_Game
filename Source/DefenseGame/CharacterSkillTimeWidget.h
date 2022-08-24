// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSkillTimeWidget.generated.h"

class UOverlay;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UCharacterSkillTimeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

public:
	UFUNCTION()
	void OnChangeCooldownState(float Time);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* BaseOverlay;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* BackgroundImage;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* KeyText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* CooldownImage;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* TimeText;

public:
	void SetKeyText(FString text);
	void SetCooldownVisibility(bool visible);
};
