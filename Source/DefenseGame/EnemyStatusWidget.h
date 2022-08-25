// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStatusWidget.generated.h"

class UOverlay;
class UProgressBar;

/**
 * 
 */
UCLASS()
class DEFENSEGAME_API UEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

public:
	UFUNCTION()
	void OnChangeHP(float CurrentHP, float MaxHP);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* BaseOverlay;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UProgressBar* HealthBar;
};
