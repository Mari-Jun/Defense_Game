// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenseObject.h"
#include "Enemy.generated.h"

class UEnemyStatusWidget;
class UEnemyDamageWidget;
class AEnemyController;
class ABaseCharacter;
class AItem;

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
	float DefaultSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackSpeed = 350.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReactionSpeed = 250.f;
};

UCLASS()
class DEFENSEGAME_API AEnemy : public ADefenseObject
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
	virtual void KillObject() override;
	virtual void DestoryEnemy();
	virtual void DropItem();

protected:
	virtual void ChangeEnemyState(EEnemyState State);

	virtual void ShowStatusWidget();
	virtual void HideStatusWidget();
	virtual void PlayHitReaction(float HitYaw) override;

	virtual bool CheckAttack();
	virtual void Attack();

	UFUNCTION()
	void AddDamageNumber(float HPDamage, float ShieldDamage, bool IsCritical);
	UFUNCTION()
	void DestoryDamageNumber(UEnemyDamageWidget* DamageWidget);
	void RenderHitNumbers();

	UFUNCTION()
	virtual void OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void DisableCollision() override;

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

	TSet<AActor*> InAttackRangeActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* EnemyStatusWidgetComponent;

	UEnemyStatusWidget* EnemyStatusWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ShowStatusWidgetTime = 5.0f;
	FTimerHandle ShowStatusWidgetTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DamageWidgetSpawnPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ShowDamageWidgetTime = 2.0f;
	UPROPERTY(VisibleAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TMap<UEnemyDamageWidget*, FVector> DamageNumbers;
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UEnemyDamageWidget> DamageNumberWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DeathTime = 3.0f;
	FTimerHandle DestoryActorTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USphereComponent* AttackRangeSphereComponent;

	/** 0% ~ 100%*/
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 ItemDropProbability = 50;
	/** 아이템, 아이템 확률 (숫자가 크면 클수록 확률이 높음)*/
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<AItem>, int32> DropItems;

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

public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
