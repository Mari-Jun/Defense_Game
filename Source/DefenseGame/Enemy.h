// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Enemy.generated.h"

class UEnemyStatusWidget;
class AEnemyController;
class ABaseCharacter;

class UBlendSpace1D;
class UWidgetComponent;
class UBehaviorTree;
class USphereComponent;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ENone UMETA(DisplayName = "None"),
	EReaction UMETA(DisplayName = "Reaction"),
	EAttack UMETA(DisplayName = "Attack"),
	EDeath UMETA(DisplayName = "Death"),

	EMAX UMETA(DisplayName = "MAX"),
};

USTRUCT(BlueprintType)
struct FEnemyStatusData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHP = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHP = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefaultSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeed = 350.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReactionSpeed = 250.f;

	float CurrentReactionValue = 0.f;
	/** ����ġ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReactionValue = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 20.f;
	bool CanAttack = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeEnemyHPDelegate, float, CurrentHP, float, MaxHP);

UCLASS()
class DEFENSEGAME_API AEnemy : public ACharacter, public IGenericTeamAgentInterface
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

	void ApplyDamage(AActor* OtherActor, float Damage);

	UFUNCTION(BlueprintCallable)
	virtual void FinishDeath();

private:
	virtual void KillEnemy();
	virtual void DestoryEnemy();

protected:
	virtual void ChangeEnemyState(EEnemyState State);

	virtual void ShowStatusWidget();
	virtual void HideStatusWidget();
	virtual void PlayHitReaction(float HitYaw);

	virtual bool CheckAttack();
	virtual void Attack();

	UFUNCTION()
	virtual void OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void DisableCollision();

public:
	UFUNCTION(BlueprintCallable)
	virtual void AttackHit() {}
	UFUNCTION(BlueprintCallable)
	virtual void AttackHitStart() {}
	UFUNCTION(BlueprintCallable)
	virtual void AttackHitEnd() {}
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void ReactionEnd();

private:
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	AEnemyController* EnemyController;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::ENone;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USphereComponent* AttackRangeSphereComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackAnimMontange;

	FGenericTeamId TeamId{ 0 };

public:
	FChangeEnemyHPDelegate ChangeHPDelegate;

public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
};
