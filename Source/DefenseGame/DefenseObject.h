// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "DefenseObject.generated.h"

USTRUCT(BlueprintType)
struct FCombatStatus
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentHP = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHP = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentShield = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxShield = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Attack = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Defense = 50.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Critical = 0.f;
	
	float CurrentReactionValue = 0.f;
	/** °æÁ÷Ä¡ */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReactionValue = 50.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeHPDelegate, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeShieldDelegate, float, CurrentShield, float, MaxShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeReactionDelegate, float, CurrentReaction, float, MaxReaction);

UCLASS()
class DEFENSEGAME_API ADefenseObject : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefenseObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamage(float Attack, float Critical, float HitYaw);
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
	FCombatStatus CombatStatus;

	FChangeHPDelegate ChangeHPDelegate;
	FChangeShieldDelegate ChangeShieldDelegate;
	FChangeReactionDelegate ChangeReactionDelegate;

	FGenericTeamId TeamId{ 0 };

public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
};
