// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyDamageWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UEnemyDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* DamageText;

	float TextAlpha = 1.0f;

public:
	void SetDamageText(float Damage);
	void SetTextAlpha(float Alpha);
};
