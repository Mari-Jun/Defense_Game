// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenseObject.h"
#include "Engine/DataTable.h"
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
class UShapeComponent;
class UBoxComponent;

class UNiagaraSystem;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 20, UIMin = 1, UIMax = 20))
	int32 Level = 1;
};

USTRUCT(BlueprintType)
struct FEnemyStatusTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	float HP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	float Shield;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	float Attack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	float Defense;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0))
	float ReactionValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 20))
	int32 DropItemLevel = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1))
	int32 DropCoin = 1;
};

USTRUCT(BlueprintType)
struct FEnemyAbilityData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AbilityTime = 10.0f;
	FTimerHandle AbilityTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackRatio = 1.0f;

	TSet<AActor*> InAttackRangeActors;
	bool CanTowerAttack = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UShapeComponent* AbilityEnableRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UAnimMontage*> AbilityAnimMontange;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillEnemyEventDelegate);

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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void ApplyDamage(AActor* OtherActor, float Damage) override;

	UFUNCTION(BlueprintCallable)
	virtual void FinishDeath();

	virtual void GetNewTarget();
	virtual void LoseTarget();

protected:
	virtual void KillObject() override;
	virtual void DestoryEnemy();
	virtual void DropItem();

protected:
	virtual void ChangeEnemyState(EEnemyState State);

	virtual void ShowStatusWidget();
	virtual void HideStatusWidget();
	virtual void PlayHitReaction(float HitYaw) override;

	//Attack & Ability
	virtual bool CheckAttack();
	virtual void Attack();

	template <class Shape>
	void AddNewAbility(FString AbilityName, float AbilityTime, float AttackRatio, int32 Order, TArray<FString>&& AnimationNames = {})
	{
		AbilityMap.Add(AbilityName);
		AbilityOrder.Add(AbilityName, Order);
		AbilityMap[AbilityName].AbilityTime = AbilityTime;
		AbilityMap[AbilityName].AttackRatio = AttackRatio;
		FString RangeName = AbilityName + "Range";
		AbilityMap[AbilityName].AbilityEnableRange = CreateDefaultSubobject<Shape>(FName(*RangeName));
		AbilityMap[AbilityName].AbilityEnableRange->SetupAttachment(GetRootComponent());
		AbilityMap[AbilityName].AbilityEnableRange->SetCollisionProfileName("EnemyAttack");
		AddInstanceComponent(AbilityMap[AbilityName].AbilityEnableRange);
		for (const FString& Name : AnimationNames)
		{
			AbilityMap[AbilityName].AbilityAnimMontange.Add(Cast<UAnimMontage>(StaticLoadObject(UAnimMontage::StaticClass(), nullptr,
				*FString::Printf(TEXT("%s"), *Name))));
		}
	}
	virtual void SetAbilitysCollision(bool bEnable);
	virtual void SetAbilityCollision(FString AbilityName, bool bEnable);
	virtual void StartAbilityCooldown(FString AbilityName);
	virtual void ResetAbilityTimer(FString AbilityName);
	virtual void ClearAbilityMap();

	UFUNCTION()
	virtual void OnAbilityRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnAbilityRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//DamageUI
	UFUNCTION()
	void AddDamageNumber(float HPDamage, float ShieldDamage, bool IsCritical);
	UFUNCTION()
	void DestoryDamageNumber(UEnemyDamageWidget* DamageWidget);
	void RenderHitNumbers();



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


protected:
	AEnemyController* EnemyController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::ENone;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FEnemyStatusData EnemyStatusData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyStatusTable;
	int32 DropCoin = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* EnemyStatusWidgetComponent;

	UEnemyStatusWidget* EnemyStatusWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ShowStatusWidgetTime = 5.0f;
	FTimerHandle ShowStatusWidgetTimerHandle;

	//Damage Widget
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

	//Item
	/** 0% ~ 100%*/
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 ItemDropProbability = 50;
	/** 아이템, 아이템 확률 (숫자가 크면 클수록 확률이 높음)*/
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<AItem>, int32> DropItems;
	UNiagaraSystem* DropCoinEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	FVector DropCoinEffectScale = FVector(1.f);

	//Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TMap<FString, FEnemyAbilityData> AbilityMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TMap<FString, int32> AbilityOrder;
	FString CurrentAbilityName;


	//Animation
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
	FKillEnemyEventDelegate KillEnemyEventDelegate;

public:
	EEnemyState GetEnemyState() const { return EnemyState; }
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	void SetEnemyLevel(int32 Level) { EnemyStatusData.Level = Level; }
};
