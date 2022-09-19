// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "CombatHelperInterface.h"
#include "DefenseObject.generated.h"

UCLASS()
class DEFENSEGAME_API ADefenseObject : public ACharacter, public IGenericTeamAgentInterface, public ICombatHelperInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefenseObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void TakeDamage(float Attack, float Critical, float HitYaw);
	virtual void PlayHitReaction(float HitYaw);

	virtual void KillObject() {}

	virtual TSubclassOf<UDamageType> GetDamageTypeClass() const;
	virtual void EnableCollision();
	virtual void DisableCollision();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void ApplyDamage(AActor* OtherActor, float Damage);
	UFUNCTION()
	virtual void ApplyPointDamage(AActor* OtherActor, float Damage, const FVector& HitFromDirection, const FHitResult& HitInfo);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FReactionCombatStatus CombatStatus;

	FChangeHPDelegate ChangeHPDelegate;
	FChangeShieldDelegate ChangeShieldDelegate;
	FChangeReactionDelegate ChangeReactionDelegate;
	FBroadcastDamageInfo BroadcastDamageInfoDelegate;
	FChangeAttackDelegate ChangeAttackDelegate;
	FChangeDefenseDelegate ChangeDefenseDelegate;
	FChangeCriticalDelegate ChangeCriticalDelegate;

	FGenericTeamId TeamId{ 0 };

public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusCurrentHP(float DeltaCurrentHP);
	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusMaxHP(float DeltaMaxHP);
	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusCurrentShield(float DeltaCurrentShield);
	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusMaxShield(float DeltaMaxShield);
	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusAttack(float DeltaAttack);
	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusDefense(float DeltaDefense);
	UFUNCTION(BlueprintCallable)
	void ChangeCombatStatusCritical(float DeltaCritical);
};
