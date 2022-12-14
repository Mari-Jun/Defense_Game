// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenseObject.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UBlendSpace1D;

class UCrosshairWidget;
class UCharacterStatusWidget;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	EDefault UMETA(DisplayName = "Default"),
	EAttack UMETA(DisplayName = "Attack"),
	EReaction UMETA(DisplayName = "Reaction"),
	EDeath UMETA(DisplayName = "Death"),

	EMAX UMETA(DisplayName = "MAX"),
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* HitReactionFWDAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* HitReactionRightAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* HitReactionLeftAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* HitReactionBWDAnimMontage;
};

USTRUCT(BlueprintType)
struct FCharacterStatusData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<float> AbilityTime = {3, 3, 3, 3};
	TArray<FTimerHandle> AbilityTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RespawnTime = 10.0f;
	FTimerHandle RespawnTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Coin = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeCooldownTimeDelegate, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitShakeCameraDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeCoinDelegate, int32, CoinValue);

UCLASS()
class DEFENSEGAME_API ABaseCharacter : public ADefenseObject
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	virtual void Attack();
	virtual void AbilityQ(int32 AbilityIndex);
	virtual void AbilityE(int32 AbilityIndex);
	virtual void AbilityR(int32 AbilityIndex);
	virtual void AbilityRMB(int32 AbilityIndex);

	void ResetAbilityTimer(int32 AbilityIndex);

	virtual void KillObject() override;
	virtual void RespawnCharacter();

protected:
	virtual void AttackLMBHit() {}
	virtual void AbilityQHit() {}
	virtual void AbilityEHit() {}
	virtual void AbilityRHit() {}
	virtual void AbilityRMBHit() {}

	bool CheckAbilityCooldown(int32 AbilityIndex) const;
	void StartAbilityCooldown(int32 AbilityIndex);

	virtual void PlayHitReaction(float HitYaw) override;

	UFUNCTION()
	virtual void ShakePlayerCamera();

public:
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	virtual void AttackHit();
	UFUNCTION(BlueprintCallable)
	virtual void FinishDeath();
	UFUNCTION(BlueprintCallable)
	virtual void ReactionEnd();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::EDefault;

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
	TArray<FChangeCooldownTimeDelegate> ChangeAbilityTimeDelegate;

	UPROPERTY()
	FHitShakeCameraDelegate HitShakeCameraDelegate;
	FChangeCoinDelegate ChangeCoinDelegate;

public:
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCurveFloat* GetTurnRotationCurve() const { return CharacterAnimationData.TurnRotationCurve; }
	ECharacterState GetCharacterState() const { return CharacterState; }
	void SetCharacterState(ECharacterState State);

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	void ChangeCoin(int32 DeltaCoin);
	int32 GetCoin() const;
};
