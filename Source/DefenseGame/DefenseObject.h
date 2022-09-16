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

	virtual void ApplyDamage(AActor* OtherActor, float Damage);

	virtual void DisableCollision();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FReactionCombatStatus CombatStatus;

	FChangeHPDelegate ChangeHPDelegate;
	FChangeShieldDelegate ChangeShieldDelegate;
	FChangeReactionDelegate ChangeReactionDelegate;
	FBroadcastDamageInfo BroadcastDamageInfoDelegate;

	FGenericTeamId TeamId{ 0 };

public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
};
