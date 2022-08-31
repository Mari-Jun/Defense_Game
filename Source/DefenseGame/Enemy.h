// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UEnemyStatusWidget;
class AEnemyController;

class UBlendSpace1D;
class UWidgetComponent;
class UBehaviorTree;

USTRUCT(BlueprintType)
struct FEnemyStatusData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHP = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHP = 200.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeEnemyHPDelegate, float, CurrentHP, float, MaxHP);

UCLASS()
class DEFENSEGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void FinishDeath();

private:
	virtual void KillEnemy();
	virtual void DestoryEnemy();

protected:
	virtual void ShowStatusWidget();
	virtual void HideStatusWidget();
	virtual void PlayHitReaction(float HitYaw);

private:
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	AEnemyController* EnemyController;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FEnemyStatusData EnemyStatusData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* EnemyStatusWidgetComponent;

	UEnemyStatusWidget* EnemyStatusWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ShowStatusWidgetTime = 5.0f;
	FTimerHandle ShowStatusWidgetTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DeathTime = 3.0f;
	FTimerHandle DestoryActorTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* IdleAnimSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeadAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionFWDAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionRightAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionLeftAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactionBWDAnimMontage;

public:
	FChangeEnemyHPDelegate ChangeHPDelegate;

public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

};
