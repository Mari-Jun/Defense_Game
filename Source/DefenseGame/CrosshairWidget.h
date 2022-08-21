// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

class UCanvasPanel;
class UCanvasPanelSlot;
class UImage;
/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	void SetCrosshairThickness(UCanvasPanelSlot* PanelSlot, bool IsVerticle);
	void SetCrosshairOffset(UCanvasPanelSlot* PanelSlot, bool IsVerticle, bool IsInverse);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* LeftTexture;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* RightTexture;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* TopTexture;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* BottomTexture;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor CrosshairColor = {1.f, 1.f, 1.f, 1.f};
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CrosshairThickness = 2.f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float CrosshairOfffset = 20.f;
};
