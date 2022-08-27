// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UBlendSpace1D;

class UCrosshairWidget;
class UCharacterStatusWidget;

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	ENone UMETA(DisplayName = "None"),
	EAttackLMB UMETA(DisplayName = "AttackLMB"),
	EAbilityQ UMETA(DisplayName = "AbilityQ"),
	EAbilityE UMETA(DisplayName = "AbilityE"),
	EAbilityR UMETA(DisplayName = "AbilityR"),
	EAbilityRMB UMETA(DisplayName = "AbilityRMB"),

	EMAX UMETA(DisplayName = "MAX"),
};

USTRUCT(BlueprintType)
struct FCharacterAnimationData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* IdleAnimSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* TurnLeft90Sequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* TurnRight90Sequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* TurnRotationCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace1D* JogStartBlendSpace;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace1D* JogBlendSpace;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBlendSpace1D* JogStopBlendSpace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* JumpStartSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* JumpApexSequence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* JumpLandSequence; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) /**Use Additive Sequence*/
	UAnimSequence* JumpRecoverySequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> AttackAnimMontange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AbilityQMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AbilityEMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AbilityRMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AbilityRMBMontage;
};

USTRUCT(BlueprintType)
struct FCharacterStatusData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHP = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHP = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<float> AbilityTime = {3, 3, 3, 3};
	TArray<FTimerHandle> AbilityTimerHandle;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeCharacterHPDelegate, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeCooldownTimeDelegate, float, Time);

UCLASS()
class DEFENSEGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void Attack();
	virtual void AbilityQ(int32 AbilityIndex);
	virtual void AbilityE(int32 AbilityIndex);
	virtual void AbilityR(int32 AbilityIndex);
	virtual void AbilityRMB(int32 AbilityIndex);

	void ResetAbilityTimer(int32 AbilityIndex);

protected:
	virtual void AttackLMBHit() {}
	virtual void AbilityQHit() {}
	virtual void AbilityEHit() {}
	virtual void AbilityRHit() {}
	virtual void AbilityRMBHit() {}

	bool CheckAbilityCooldown(int32 AbilityIndex) const;
	void StartAbilityCooldown(int32 AbilityIndex);

public:
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	virtual void AttackHit();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;
	UPROPERTY()
	UCrosshairWidget* CrosshairWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCharacterStatusWidget> StatusWidgetClass;
	UPROPERTY()
	UCharacterStatusWidget* StatusWidget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FCharacterAnimationData CharacterAnimationData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FCharacterStatusData CharacterStatusData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EAttackState AttackState = EAttackState::ENone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsAttackFull = false;

public:
	UPROPERTY()
	FChangeCharacterHPDelegate ChangeHPDelegate;

	UPROPERTY()
	TArray<FChangeCooldownTimeDelegate> ChangeAbilityTimeDelegate;

public:
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCurveFloat* GetTurnRotationCurve() const { return CharacterAnimationData.TurnRotationCurve; }
};
