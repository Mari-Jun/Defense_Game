// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStatusWidget.generated.h"

class ABaseCharacter;
class UCharacterProgressBarWidget;
class UCharacterSkillTimeWidget;
class UCharacterRespawnWidget;

class UCanvasPanel;
class UCanvasPanelSlot;
class UImage;
class UProgressBar;
class UTextBlock;
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
	UFUNCTION()
	void OnChangeShield(float CurrentShield, float MaxShield);

	UFUNCTION()
	void OnHitReaction();

	void SetRespawnWidgetVisibility(bool IsVisible);
	void SetRespawnTime(float Time, float MaxTime);

	UFUNCTION()
	void OnChangeCoin(int32 CoinValue);
	UFUNCTION()
	void OnChangeAttack(float AttackValue);
	UFUNCTION()
	void OnChangeDefense(float DefenseValue);
	UFUNCTION()
	void OnChangeCritical(float CriticalValue);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterProgressBarWidget* HPProgressBar;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterProgressBarWidget* ShieldProgressBar;
	

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityQWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityEWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityRWidget;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterSkillTimeWidget* AbilityRMBWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* HitImage;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UCurveLinearColor* HitImageLinearColorCurve;
	FTimerHandle HitImageColorTimer;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UCharacterRespawnWidget* RespawnWidget;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* CoinText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* AttackText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* DefenseText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* CriticalText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* BaseCharacter;

	APostProcessVolume* PostProcessVolume;

public:
	void SetBaseCharacter(ABaseCharacter* Character);

	UCharacterSkillTimeWidget* GetAbilityWidget(int32 AbilityIndex) const;
};
